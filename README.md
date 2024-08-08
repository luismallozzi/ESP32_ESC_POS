This version of the ESP32_ESC_POS library provides comprehensive control over ESC/POS thermal printers using an ESP32 via Wi-Fi. Key features include:  
Connection Management: Establish and terminate connections with the printer.
Basic Printing Functions: Print text, feed paper, and cut paper.
Text Formatting: Set bold text, text size, character set, and code page.
Buffer Management: Efficiently manage a buffer for sending commands to the printer.
Character Verification: Ensure text contains only supported characters.
Special Character Printing: Print special characters based on the selected code page and character set.
The library is designed to be easy to use and integrate into ESP32 projects, providing a robust solution for thermal printing needs.

### `ESP32_ESC_POS` Library

This version of the `ESP32_ESC_POS` library provides comprehensive control over ESC/POS thermal printers using an ESP32 via Wi-Fi. Key features include:

- **Connection Management**: Establish and terminate connections with the printer.
- **Basic Printing Functions**: Print text, feed paper, and cut paper.
- **Text Formatting**: Set bold text, text size, character set, and code page.
- **Buffer Management**: Efficiently manage a buffer for sending commands to the printer.
- **Character Verification**: Ensure text contains only supported characters.
- **Special Character Printing**: Print special characters based on the selected code page and character set.

The library is designed to be easy to use and integrate into ESP32 projects, providing a robust solution for thermal printing needs.

### Instructions for Using `ESP32_ESC_POS` Library

1. **Include the Library**:
   Include the `ESP32_ESC_POS` library in your project.

   ```cpp
   #include "ESP32_ESC_POS.h"
   ```

2. **Initialize the Library**:
   Create an instance of the `ESP32_ESC_POS` class with the IP address and port of your thermal printer.

   ```cpp
   ESP32_ESC_POS printer("192.168.0.100", 9100);
   ```

3. **Connect to the Printer**:
   Use the `connect` method to establish a connection with the printer.

   ```cpp
   if (printer.connect()) {
       Serial.println("Connected to printer");
   } else {
       Serial.println("Failed to connect to printer");
   }
   ```

4. **Initialize the Printer**:
   Initialize the printer to set it to a known state.

   ```cpp
   printer.initialize();
   ```

5. **Print Text**:
   Use the `print` method to print text.

   ```cpp
   printer.print("Hello, World!");
   ```

6. **Feed Paper**:
   Use the `feed` method to feed a specified number of lines.

   ```cpp
   printer.feed(3);
   ```

7. **Cut Paper**:
   Use the `cut` method to cut the paper.

   ```cpp
   printer.cut();
   ```

8. **Set Text Formatting**:
   - **Bold Text**: Use the `setBold` method to enable or disable bold text.

     ```cpp
     printer.setBold(true); // Enable bold text
     printer.setBold(false); // Disable bold text
     ```

   - **Text Size**: Use the `setTextSize` method to set the text size (1 to 8).

     ```cpp
     printer.setTextSize(2); // Set text size to 2
     ```

9. **Set Character Set and Code Page**:
   - **Character Set**: Use the `setCharset` method to set the character set.

     ```cpp
     printer.setCharset(0); // Set character set to 0
     ```

   - **Code Page**: Use the `setCodePage` method to set the code page.

     ```cpp
     printer.setCodePage(0); // Set code page to 0
     ```

10. **Read Printer Response**:
    Use the `readResponse` method to read the response from the printer.

    ```cpp
    String response = printer.readResponse();
    Serial.println(response);
    ```

11. **Verify Characters**:
    Use the `verifyCharacters` method to ensure the text contains only supported characters.

    ```cpp
    if (printer.verifyCharacters("Hello, World!")) {
        Serial.println("Text is valid");
    } else {
        Serial.println("Text contains unsupported characters");
    }
    ```

12. **Print Special Characters**:
    Use the `printSpecialCharacters` method to print special characters based on the selected code page and character set.

    ```cpp
    printer.printSpecialCharacters(0, 0); // Print special characters for code page 0 and charset 0
    ```

### Example Usage

```cpp
#include <WiFi.h>
#include "ESP32_ESC_POS.h"

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

ESP32_ESC_POS printer("192.168.0.100", 9100);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    if (printer.connect()) {
        Serial.println("Connected to printer");
        printer.initialize();
        printer.setBold(true);
        printer.print("Hello, World!");
        printer.setBold(false);
        printer.feed(3);
        printer.cut();
    } else {
        Serial.println("Failed to connect to printer");
    }
}

void loop() {
    // Your code here
}
```
