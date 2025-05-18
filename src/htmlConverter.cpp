#include "../htmlConverter.h"
#include <iostream>
#include <sstream>

HTMLWriter::HTMLWriter(const std::string &filename) : filename(filename) {
    Logger::log(LogLevel::INFO, "Initializing HTMLWriter with file: " + filename);
    htmlContent = R"(
<html>
<head>
    <title>C++ Code Syntax Highlighting</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        .container {
            display: flex;
            flex-direction: row;
        }
        .sidebar {
            width: 250px;
            padding: 20px;
            background-color: #f4f4f4;
            border-right: 2px solid #ccc;
            height: 100vh;
        }
        .main {
            flex-grow: 1;
            padding: 20px;
            background-color: #fff;
        }
        .console {
                position: fixed;
                bottom: 0;
                left: 0;
                right: 0;
                height: 250px;
                background-color: #ffffff;
                color: #000000;
                font-family: monospace;
                border-top: 2px solid #ccc;
                padding: 10px 20px;
                overflow-y: auto;
                z-index: 1000;
                transition: background-color 0.3s, color 0.3s;
            }
        .groupItem {
            padding: 5px;
            font-weight: 500;
            display: block;
            margin-bottom: 10px;
        }
    </style>
</head>
<body>
<div class="container">
    <div class="sidebar">
        <h2>Legend</h2>
        <div class="groupItem" style="color:#ff7f50; font-weight:900;">Group 0: C++ Keywords</div>
        <div class="groupItem" style="color:#6a5acd; font-weight:900;">Group 1: String Types</div>
        <div class="groupItem" style="color:#4169e1; font-weight:900;">Group 2: C++ Containers</div>
        <div class="groupItem" style="color:#3cb371; font-weight:900;">Group 3: RegEx Pattern</div>
    </div>
    <div class="main">
)";

}

HTMLWriter::~HTMLWriter() {
    Logger::log(LogLevel::INFO, "Closing HTMLWriter and saving file: " + filename);
    if (file.is_open()) {
        file.close();
    }
}

void HTMLWriter::writePlainText(const std::string &text) {
    Logger::log(LogLevel::DEBUG, "Writing plain text: " + text);
    std::string escapedText = escapeHTML(text);
    htmlContent += escapedText + " ";  // Add a space after each word to avoid word break issues
}

void HTMLWriter::writeStyledText(const std::string &text, const std::string &hexColorCode, int fontWeight) {
    Logger::log(LogLevel::DEBUG, "Writing styled text: " + text + " | Color: " + hexColorCode + " | Font Weight: " + std::to_string(fontWeight));

    std::string escapedText = escapeHTML(text);
    htmlContent += "<span style=\"color:" + hexColorCode +
                   "; font-weight:" + std::to_string(fontWeight) +
                   ";\">" + escapedText + "</span> ";
}

void HTMLWriter::addNewline() {
    Logger::log(LogLevel::DEBUG, "Adding newline to HTML output.");
    htmlContent += "<br>";
}

void HTMLWriter::saveFile() {
    Logger::log(LogLevel::INFO, "Saving HTML file: " + filename);

    // Add a header and some introduction text
    htmlContent += "</div></div>"; // closes .main and .container
    htmlContent += R"(
            <div class="console">
                <h2>Output Log</h2>
                <pre>
                    )" + errorMessage + R"(
                </pre>
            </div>
            </body></html>
            )";


    // Print the full content before saving
    std::string content = "========== HTML CONTENT BEFORE SAVING ==========";
    content += htmlContent;
    content += "================================================";
    Logger::log(LogLevel::INFO, content);

    // Open the file for writing (no "../" to ensure it's saved in the current directory)
    file.open(filename);
    if (file.is_open()) {
        // Write the full content to the file
        file << htmlContent;
        Logger::log(LogLevel::SUCCESS, "HTML file saved successfully: " + filename);
        file.close();  // Ensure the file is properly closed after writing
    } else {
        Logger::log(LogLevel::ERROR, "Could not open file for writing: " + filename);
    }
}

std::string HTMLWriter::escapeHTML(const std::string &text) {
    std::ostringstream escapedText;

    for (char ch : text) {
        switch (ch) {
            case '<':
                escapedText << "&lt;";
                break;
            case '>':
                escapedText << "&gt;";
                break;
            case '&':
                escapedText << "&amp;";
                break;
            case '"':
                escapedText << "&quot;";
                break;
            case '\'':
                escapedText << "&apos;";
                break;
            default:
                escapedText << ch;
                break;
        }
    }

    return escapedText.str();
}

void HTMLWriter::addError(const std::string &text) {
    Logger::log(LogLevel::INFO, "Adding error message to HTML output: " + text);
    errorMessage += "<div class=\"error\">" + escapeHTML(text) + "</div>";
    errorMessage += "<br>";
}
