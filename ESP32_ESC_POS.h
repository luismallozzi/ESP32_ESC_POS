#ifndef ESP32_ESC_POS_H
#define ESP32_ESC_POS_H

#include <WiFi.h>
#include <WiFiClient.h>

#define BUFFER_SIZE 512

class ESP32_ESC_POS {
public:
    ESP32_ESC_POS(const char* ip, uint16_t port);
    bool connect();
    void disconnect();
    void initialize();
    void print(const char* text);
    void feed(uint8_t lines);
    void cut();
    void setBold(bool bold);
    void setTextSize(uint8_t size);
    void setCharset(uint8_t charset);
    void setCodePage(uint8_t codePage);
    String readResponse();
    bool verifyCharacters(const char* text);
    void printSpecialCharacters(uint8_t codePage, uint8_t charset);

private:
    const char* _ip;
    uint16_t _port;
    WiFiClient _client;
    uint8_t _buffer[BUFFER_SIZE];
    size_t _bufferIndex;

    void clearBuffer();
    void addToBuffer(uint8_t data);
    void sendBuffer();
    String convertToISO8859_1(const char* text);
};

#endif