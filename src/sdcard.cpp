#if defined(TEENSYDUINO)

#include <debug.h>

#include "sdcard.h"

// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = SS;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif  // SDCARD_SS_PIN

#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

static ArduinoOutStream cout(Serial);

sdcard::sdcard() {
}

byte sdcard::setup() {
 #if DEBUG_SDCARD
  Serial.print("SD_CS_PIN: ");
  Serial.println(SD_CS_PIN);

  Serial.print("HAS_SDIO_CLASS: ");
  Serial.println(HAS_SDIO_CLASS);
  Serial.print("ENABLE_DEDICATED_SPI: ");
  Serial.println(ENABLE_DEDICATED_SPI);
  Serial.print("ENABLE_DEDICATED_SPI: ");
  Serial.println(ENABLE_DEDICATED_SPI);
#endif

  uint32_t t = millis();
  if (!_sd.cardBegin(SD_CONFIG)) {
    cout << F(
           "\n\tSD initialization failed.\n"
           "\tDo not reformat the card!\n"
           "\tIs the card correctly inserted?\n"
           "\tIs there a wiring/soldering problem?\n");
    if (isSpi(SD_CONFIG)) {
      cout << F(
           "\tIs SD_CS_PIN set to the correct value?\n"
           "\tDoes another SPI device need to be disabled?\n"
           );
    }
    errorPrint();
    return 1;
  }
  t = millis() - t;
  cout << F("\tSD Card init time: ") << t << " ms" << endl;

  return cardInfo();
}

byte sdcard::cardInfo() {
  if (!_sd.card()->readCID(&_cid) ||
      !_sd.card()->readCSD(&_csd) ||
      !_sd.card()->readOCR(&_ocr)) {
    cout << F("\tReadInfo failed\n");
    errorPrint();
    return 1;
  }

  printCardType();
  printCid();
  printCsd();

  cout << F("\n\tOCR: ") << uppercase << showbase;
  cout << hex << _ocr << dec << endl;
  if (!printMbr()) {
    errorPrint();
    return 1;
  }

  if (!_sd.volumeBegin()) {
    cout << F("\n\tVolume begin failed. Is the card formatted?\n");
    errorPrint();
    return 1;
  }
  printVolume();

  return 0;
}

bool sdcard::deleteFile(const char* filePath) {
  if (!_sd.remove(filePath)) {
    Serial.println("Error: File deletion failed.");
    return false;
  }

  return true;
}

FsFile sdcard::openRead(const char* filePath) {
  // return _volume.open(fileName);
  FsFile file;
  // if (!file.open(filePath, FILE_READ)) {
  //   _sd.errorHalt(&Serial, F("Failed to open file for reading."));
  //   file.close();
  //   return file;
  // }

  return file;
}

FsFile sdcard::openWrite(const char* filePath) {
  FsFile file;

  // if (!file.open(filePath, O_WRONLY | O_CREAT | O_TRUNC)) {
  //   _sd.errorHalt(&Serial, F("Failed to open file for writing."));
  //   return file;
  // }

  return file;
}

bool sdcard::readJson(const char* filePath, JsonDocument& doc) {
  Serial.print("readJson: Attempting to read the file: ");
  Serial.println(filePath);

  Serial.print("readJson: exists? ");
  Serial.println(_sd.exists(filePath));
  if (!_sd.exists(filePath)) {
    Serial.print("readJson: File does not exist: ");
    Serial.println(filePath);
    return false;
  }

  FsFile jsonFile = _sd.open(filePath, O_READ);
  if (!jsonFile) {
    // _sd.errorHalt(&Serial, F("Failed to open file for writing."));
    Serial.print("readJson: Failed to open file: ");
    Serial.println(filePath);
    return false;
  }

  Serial.print("readJson: open? ");
  Serial.println(jsonFile.isOpen());
  if (!jsonFile.isOpen()) {
    // _sd.errorHalt(&Serial, F("Failed to open file for writing."));
    Serial.print("readJson: File is not open: ");
    Serial.println(filePath);
    return false;
  }

  DeserializationError error = deserializeJson(doc, jsonFile);
  jsonFile.close();

  Serial.println("readJson: json: ");
  serializeJson(doc, Serial);

  if (error) {
    Serial.print("readJson: Deserialize Json failed: ");
    Serial.println(error.f_str());
    return false;
  }

  return true;
}

bool sdcard::writeJson(const char* filePath, JsonDocument& doc) {
  Serial.print("writeJson: Attempting to write the file: ");
  Serial.println(filePath);

  FsFile jsonFile = _sd.open(filePath, O_WRITE | O_CREAT | O_TRUNC);
  if (!jsonFile) {
    // _sd.errorHalt(&Serial, F("Failed to open file for writing."));
    Serial.print("writeJson: Failed to open file: ");
    Serial.println(filePath);
    return false;
  }

  Serial.print("writeJson: open? ");
  Serial.println(jsonFile.isOpen());
  if (!jsonFile.isOpen()) {
    // _sd.errorHalt(&Serial, F("Failed to open file for writing."));
    Serial.print("writeJson: File is not open: ");
    Serial.println(filePath);
    return false;
  }

  size_t bytes_written = serializeJson(doc, jsonFile);
  Serial.print("writeJson: bytes_written: ");
  Serial.println(bytes_written);
  jsonFile.close();

  if (bytes_written <= 0) {
    Serial.println("writeJson: Failed to write JSON data.");
    return false;
  }

  return true;
}

void sdcard::clearSerialInput() {
  uint32_t m = micros();
  do {
    if (Serial.read() >= 0)
      m = micros();
  } while (micros() - m < 10000);
}

void sdcard::errorPrint() {
  if (!_sd.sdErrorCode())
    return;

  cout << F("\tSD errorCode: ") << hex << showbase;
  printSdErrorSymbol(&Serial, _sd.sdErrorCode());
  cout << F(" = ") << int(_sd.sdErrorCode()) << endl;
  cout << F("SD errorData = ") << int(_sd.sdErrorData()) << endl;
}

bool sdcard::printCid() {
  cout << F("\n\tManufacturer ID: ");
  cout << uppercase << showbase << hex << int(_cid.mid) << dec << endl;

  cout << F("\tOEM ID: ") << _cid.oid[0] << _cid.oid[1] << endl;

  cout << F("\tProduct: ");
  for (uint8_t i = 0; i < 5; i++)
    cout << _cid.pnm[i];

  cout << F("\n\tVersion: ");
  cout << int(_cid.prv_n) << '.' << int(_cid.prv_m) << endl;

  cout << F("\tSerial number: ") << hex << _cid.psn << dec << endl;

  cout << F("\tManufacturing date: ");
  cout << int(_cid.mdt_month) << '/';
  cout << (2000 + 16*_cid.mdt_year_high + _cid.mdt_year_low) << endl;
  cout << endl;

  return true;
}

bool sdcard::printCsd() {
  bool eraseSingleBlock;
  if (_csd.v1.csd_ver == 0) {
    eraseSingleBlock = _csd.v1.erase_blk_en;
    _eraseSize = (_csd.v1.sector_size_high << 1) | _csd.v1.sector_size_low;
  } 
  else if (_csd.v2.csd_ver == 1) {
    eraseSingleBlock = _csd.v2.erase_blk_en;
    _eraseSize = (_csd.v2.sector_size_high << 1) | _csd.v2.sector_size_low;
  } 
  else {
    cout << F("\tCSD version error\n");
    return false;
  }

  _eraseSize++;
  cout << F("\tcardSize: ") << 0.000512 * sdCardCapacity(&_csd);
  cout << F(" MB (MB = 1,000,000 bytes)\n");

  cout << F("\tflashEraseSize: ") << int(_eraseSize) << F(" blocks\n");

  cout << F("\teraseSingleBlock: ");
  if (eraseSingleBlock)
    cout << F("true\n");
  else
    cout << F("false\n");

  return true;
}

bool sdcard::printMbr() {
  MbrSector_t mbr;
  bool valid = true;
  if (!_sd.card()->readSector(0, (uint8_t*)&mbr)) {
    cout << F("\n\tRead MBR failed.\n");
    errorPrint();
    return false;
  }

  cout << F("\n\tSD Partition Table\n");
  cout << F("\tpart,boot,bgnCHS[3],type,endCHS[3],start,length\n");
  for (uint8_t ip = 1; ip < 5; ip++) {
    cout << "\t";
    MbrPart_t *pt = &mbr.part[ip - 1];
    if ((pt->boot != 0 && pt->boot != 0X80) || getLe32(pt->relativeSectors) > sdCardCapacity(&_csd)) {
      valid = false;
    }

    cout << int(ip) << ',' << uppercase << showbase << hex;
    cout << int(pt->boot) << ',';
    for (int i = 0; i < 3; i++ )
      cout << int(pt->beginCHS[i]) << ',';

    cout << int(pt->type) << ',';
    for (int i = 0; i < 3; i++ )
      cout << int(pt->endCHS[i]) << ',';

    cout << dec << getLe32(pt->relativeSectors) << ',';
    cout << getLe32(pt->totalSectors) << endl;
  }

  if (!valid)
    cout << F("\n\tMBR not valid, assuming Super Floppy format.\n");

  return true;
}

void sdcard::printCardType() {
  cout << F("\n\tCard type: ");

  switch (_sd.card()->type()) {
    case SD_CARD_TYPE_SD1:
      cout << F("SD1\n");
      break;

    case SD_CARD_TYPE_SD2:
      cout << F("SD2\n");
      break;

    case SD_CARD_TYPE_SDHC:
      if (sdCardCapacity(&_csd) < 70000000)
        cout << F("SDHC\n");
      else
        cout << F("SDXC\n");
      break;

    default:
      cout << F("Unknown\n");
  }
}

void sdcard::printVolume() {
  cout << F("\n\tScanning FAT, please wait.\n");
  uint32_t freeClusterCount = _sd.freeClusterCount();

  cout << F("\n\t\tVolume is ");
  if (_sd.fatType() <= 32)
    cout << F("FAT") << int(_sd.fatType()) << endl;
  else
    cout << F("exFAT\n");

  cout << F("\t\tSectors Per Cluster: ") << _sd.sectorsPerCluster() << endl;
  cout << F("\t\tCluster Count:       ") << _sd.clusterCount() << endl;
  cout << F("\t\tFree Cluster Count:  ") << freeClusterCount << endl;
  cout << F("\t\tFat Start Sector:    ") << _sd.fatStartSector() << endl;
  cout << F("\t\tData Start Sector:   ") << _sd.dataStartSector() << endl;

  if (_sd.dataStartSector() % _eraseSize) {
    cout << F("\t\tData area is not aligned on flash erase boundary!\n");
    cout << F("\t\tDownload and use formatter from www.sdcard.org!\n");
  }
}

#endif