#include "ESP32_ESC_POS.h"

ESP32_ESC_POS::ESP32_ESC_POS(const char* ip, uint16_t port) : _ip(ip), _port(port), _bufferIndex(0) {}

bool ESP32_ESC_POS::connect() {
    return _client.connect(_ip, _port);
}

void ESP32_ESC_POS::disconnect() {
    _client.stop();
}

void ESP32_ESC_POS::initialize() {
    clearBuffer();
    addToBuffer(0x1B); // ESC
    addToBuffer(0x40); // @
    sendBuffer();
}
void ESP32_ESC_POS::feed(uint8_t lines) {
    clearBuffer();
    addToBuffer(0x1B); // ESC
    addToBuffer(0x64); // d
    addToBuffer(lines); // number of lines
    sendBuffer();
}
void ESP32_ESC_POS::print(const char* text) {
    String convertedText = convertToISO8859_1(text);
    if (verifyCharacters(convertedText.c_str())) {
        clearBuffer();
        for (size_t i = 0; i < convertedText.length(); i++) {
            addToBuffer(convertedText[i]);
        }
        sendBuffer();
    } else {
        Serial.println("Character verification failed. Text contains unsupported characters.");
    }
}

void ESP32_ESC_POS::cut() {
    clearBuffer();
    addToBuffer(0x1D); // GS
    addToBuffer(0x56); // V
    addToBuffer(0x00); // Full cut
    sendBuffer();
}

void ESP32_ESC_POS::setBold(bool bold) {
    clearBuffer();
    addToBuffer(0x1B); // ESC
    addToBuffer(0x45); // E
    addToBuffer(bold ? 0x01 : 0x00); // 1 for bold, 0 for normal
    sendBuffer();
}

void ESP32_ESC_POS::setTextSize(uint8_t size) {
    if (size < 1) size = 1;
    if (size > 8) size = 8;
    clearBuffer();
    addToBuffer(0x1D); // GS
    addToBuffer(0x21); // !
    addToBuffer((size - 1) << 4 | (size - 1)); // size x size
    sendBuffer();
}

void ESP32_ESC_POS::setCharset(uint8_t charset) {
    clearBuffer();
    addToBuffer(0x1B); // ESC
    addToBuffer(0x52); // R
    addToBuffer(charset);
    sendBuffer();
}

void ESP32_ESC_POS::setCodePage(uint8_t codePage) {
    clearBuffer();
    addToBuffer(0x1B); // ESC
    addToBuffer(0x74); // t
    addToBuffer(codePage);
    sendBuffer();
}
String ESP32_ESC_POS::readResponse() {
    String response = "";
    while (_client.available()) {
        response += (char)_client.read();
    }
    return response;
}

bool ESP32_ESC_POS::verifyCharacters(const char* text) {
    while (*text) {
        uint8_t c = (uint8_t)*text;
        if (c < 32 && c != 10 && c != 13) {
            return false; // Caracteres de controle não imprimíveis (exceto newline e carriage return)
        }
        text++;
    }
    return true;
}

String ESP32_ESC_POS::convertToISO8859_1(const char* text) {
    String result = "";
    while (*text) {
        uint8_t c = (uint8_t)*text;
        if (c >= 0xC3 && c <= 0xC3) {
            text++;
            c = (uint8_t)*text;
            c += 0x40;
        }
        result += (char)c;
        text++;
    }
    return result;
}

void ESP32_ESC_POS::clearBuffer() {
    _bufferIndex = 0;
}

void ESP32_ESC_POS::addToBuffer(uint8_t data) {
    if (_bufferIndex < BUFFER_SIZE) {
        _buffer[_bufferIndex++] = data;
    }
}

void ESP32_ESC_POS::sendBuffer() {
    if (_bufferIndex > 0) {
        _client.write(_buffer, _bufferIndex);
        clearBuffer();
    }
}

void ESP32_ESC_POS::printSpecialCharacters(uint8_t codePage, uint8_t charset) {
    setCharset(charset);
    setCodePage(codePage);
    clearBuffer();
    addToBuffer(0x1B); // ESC
    addToBuffer(0x40); // @

    const uint8_t specialChars[] = {
            0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
            0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
            0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
            0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
            0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
            0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
            0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
            0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
    };

    for (uint8_t i = 0; i < sizeof(specialChars); i++) {
        addToBuffer(specialChars[i]);
    }
    addToBuffer(0x0A); // Nova linha
    sendBuffer();
}