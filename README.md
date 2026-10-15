# Steganography Using C

## 📌 Project Overview

Steganography is a technique used to hide secret information inside another file, such as an image, without noticeably changing its appearance.

This project implements **Image Steganography using C programming**. It allows users to hide a secret text message inside a BMP image and retrieve the hidden message later using the decoding process.

The project uses the **Least Significant Bit (LSB) technique**, in which secret message bits are stored in the least significant bits of image pixel data. Since only small changes are made to the pixel values, the encoded image looks almost identical to the original image to the human eye.

## 🎯 Project Objectives

- To understand the basic concepts of steganography.
- To hide secret text messages inside BMP images.
- To extract hidden messages from encoded images.
- To understand file handling and bitwise operations in C.
- To learn how image data can be manipulated to store information.

## ✨ Features

- **Encoding:** Hides a secret text message inside a BMP image.
- **Decoding:** Extracts the hidden message from a stego image.
- **LSB Technique:** Uses the Least Significant Bit method to store message bits.
- **BMP Image Support:** Uses BMP images as cover images.
- **Text File Input:** Reads the secret message from a text file.
- **Command-Line Execution:** Supports encoding and decoding through command-line arguments.
- **Message Retrieval:** Saves the decoded message into an output text file.

## 🛠️ Technologies Used

- **Programming Language:** C
- **File Handling:** Reading and writing image and text files.
- **Bitwise Operations:** Manipulating individual bits for encoding and decoding.
- **Image Processing:** Accessing and modifying BMP image data.
- **Dynamic Memory Management:** Managing memory where required.
- **Command-Line Arguments:** Passing input files and operation modes to the program.

## 🔍 What Is the LSB Technique?

LSB stands for **Least Significant Bit**. It is the rightmost bit in a binary number and represents the smallest value.

For example:

Original pixel value:
`11001010`

After storing one message bit:
`11001011`

Only the last bit changes. The numerical difference is just one, so the change is generally too small to be noticed visually.

By storing message bits in selected image pixel values, the program can hide text inside an image and retrieve it later.

## ⚙️ How the Project Works

### 1. Encoding (Hiding a Message)

1. The program accepts a cover BMP image and a secret text file.
2. It opens and reads the input files.
3. The secret message is converted into binary data.
4. Message bits are embedded into the image using the LSB technique.
5. The encoded image is saved as a new BMP file.

**Input:** Cover image + secret text file  
**Output:** Stego image containing the hidden message

### 2. Decoding (Extracting a Message)

1. The program accepts the encoded BMP image.
2. It reads the image data.
3. The hidden bits are extracted from the image pixels.
4. The extracted bits are reconstructed into the original text message.
5. The decoded message is saved into an output text file.

**Input:** Stego image  
**Output:** Extracted secret message

## 🔄 Project Workflow

Encoding process:

        ENCODING
           |
           v
    Cover BMP Image
           +
     Secret Text File
           |
           v
    Convert Text to Bits
           |
           v
     Apply LSB Technique
           |
           v
      Generate Stego Image
      
Decoding process: 

         DECODING
           |
           v
       Read Stego Image
           |
           v
      Extract Hidden Bits
           |
           v
    Reconstruct Secret Text
           |
           v
       Output Text File

## ▶️ How to Compile and Run

### Compile the Project

Run the following command in the project directory:

gcc *.c

This compiles the C source files and, if successful, creates the default executable `a.out` with GCC on typical Linux environments.

### Encoding — Hide a Secret Message

./a.out -e beautiful.bmp secret.txt

**Explanation:**
- -e specifies encoding mode.
- `beautiful.bmp` is the cover image.
- `secret.txt` contains the secret message.

The program generates an encoded image, according to the output filename and behavior implemented in the source code.

### Decoding — Retrieve the Hidden Message

```bash
./a.out -d stego.bmp
```

**Explanation:**
- `-d` specifies decoding mode.
- `stego.bmp` is the encoded image.

The program extracts the hidden message and saves it to an output text file, according to the implementation.

> **Note:** The encoding and decoding commands above follow this project's command-line format. The actual output filenames depend on how they are handled in the source code.

## 📥 Sample Input and Output

### Encoding

**Inputs:**
- `beautiful.bmp` — Original cover image
- `secret.txt` — File containing the secret message

**Output:**
- An encoded BMP image containing the hidden message.

### Decoding

**Input:**
- `stego.bmp` — Encoded image

**Output:**
- A text file containing the extracted secret message.

The encoded image should look visually similar to the original image when the LSB changes are small. However, the underlying pixel data may differ.

## 📚 Key Learning Outcomes

- Understanding steganography and information hiding.
- Using file operations in C.
- Working with binary data and bitwise operators.
- Learning the basics of BMP image data.
- Implementing encoding and decoding logic.
- Using command-line arguments.
- Practicing modular programming, debugging, and error handling.

## ⚠️ Limitations

- The project supports BMP images according to its implementation.
- The amount of text that can be hidden depends on the image size and encoding method.
- Encoding changes some pixel data, even if those changes are not visually noticeable.
- Steganography hides the existence of a message but does not encrypt its contents.
- If an encoded image is modified, compressed, or converted to another format, the hidden message may be damaged or lost.
- The project should not be considered a complete secure communication system without additional protection.

## 🔒 Steganography vs. Encryption

**Steganography** hides a message inside another file, while **encryption** transforms a message into an unreadable form using a cryptographic method.

This project focuses on hiding a secret message inside an image. It does not provide encryption unless a separate encryption feature is implemented.

## 🚀 Future Enhancements

- Support additional image formats.
- Add password-based access to the hidden message.
- Integrate encryption before encoding.
- Improve error handling and input validation.
- Add checks to detect corrupted or incomplete encoded data.

## 👨‍💻 Conclusion

This project demonstrates the practical implementation of image steganography using C programming. By combining file handling, bitwise operations, and BMP image processing, it shows how secret text can be embedded into an image and retrieved later.

It provides hands-on experience with low-level data manipulation and the fundamental concepts of information hiding.

**Steganography — Hide • Protect • Communicate**
