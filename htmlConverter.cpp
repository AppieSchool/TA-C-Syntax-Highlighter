#include "htmlConverter.h"
#include <iostream>
#include <sstream>

HTMLWriter::HTMLWriter(const std::string &filename) : filename(filename) {
    std::cout << "[INFO] Initializing HTMLWriter with file: " << this->filename << std::endl;
    htmlContent = "<html><head><title>C++ Code Syntax Highlighting</title><style>"
                  "body { font-family: Arial, sans-serif; margin: 20px; }"
                  "h1 { color: #333; text-align: center; }"
                  ".groupLegend { margin-top: 20px; padding: 10px; border: 1px solid #ccc; border-radius: 5px; }"
                  ".groupItem { padding: 5px; font-weight: 500; display: inline-block; margin-right: 10px; }"
                  ".tooltip { position: relative; display: inline-block; cursor: pointer; }"
                  ".tooltip .tooltiptext { visibility: hidden; width: 120px; background-color: #6c757d; color: #fff; text-align: center; border-radius: 5px; padding: 5px 0; position: absolute; z-index: 1; bottom: 100%; left: 50%; margin-left: -60px; opacity: 0; transition: opacity 0.3s; }"
                  ".tooltip:hover .tooltiptext { visibility: visible; opacity: 1; }"
                  "</style></head><body>";
}

HTMLWriter::~HTMLWriter() {
    std::cout << "[INFO] Closing HTMLWriter..." << std::endl;
    if (file.is_open()) {
        std::cout << "[INFO] Closing file: " << filename << std::endl;
        file.close();
    }
}

void HTMLWriter::writePlainText(const std::string &text) {
    std::cout << "[DEBUG] Writing plain text: " << text << std::endl;
    std::string escapedText = escapeHTML(text);
    htmlContent += escapedText + " ";  // Add a space after each word to avoid word break issues
}

void HTMLWriter::writeStyledText(const std::string &text, const std::string &hexColorCode, int fontWeight) {
    std::cout << "[DEBUG] Writing styled text: " << text
              << " | Color: " << hexColorCode
              << " | Font Weight: " << fontWeight << std::endl;

    std::string escapedText = escapeHTML(text);
    htmlContent += "<span style=\"color:" + hexColorCode +
                   "; font-weight:" + std::to_string(fontWeight) +
                   ";\">" + escapedText + "</span> ";
}

void HTMLWriter::addNewline() {
    std::cout << "[DEBUG] Adding newline to HTML output." << std::endl;
    htmlContent += "<br>";
}

void HTMLWriter::saveFile() {
    std::cout << "[INFO] Saving HTML file: " << filename << std::endl;

    // Add a header and some introduction text
    htmlContent += "<h1>C++ Code Syntax Highlighting</h1>";
    htmlContent += "<p>This page highlights various C++ keywords and types based on predefined groups.</p>";

    // Add group legend
    htmlContent += "<div class=\"groupLegend\"><h2>Legend</h2>";
    htmlContent += "<div class=\"groupItem\" style=\"color:#ff7f50; font-weight:500;\">Group 0: C++ Keywords</div>";
    htmlContent += "<div class=\"groupItem\" style=\"color:#6a5acd; font-weight:500;\">Group 1: String Types</div>";
    htmlContent += "<div class=\"groupItem\" style=\"color:#4169e1; font-weight:500;\">Group 2: C++ Containers</div>";
    htmlContent += "<div class=\"groupItem\" style=\"color:#3cb371; font-weight:900;\">Group 3: Regular Expression Pattern</div>";
    htmlContent += "</div>";

    // Add the closing tags for the HTML content
    htmlContent += "</body></html>";

    // Print the full content before saving
    std::cout << "========== HTML CONTENT BEFORE SAVING ==========" << std::endl;
    std::cout << htmlContent << std::endl;
    std::cout << "================================================" << std::endl;

    // Open the file for writing (no "../" to ensure it's saved in the current directory)
    file.open(filename);
    if (file.is_open()) {
        // Write the full content to the file
        file << htmlContent;
        std::cout << "[SUCCESS] HTML file saved successfully: " << filename << std::endl;
        file.close();  // Ensure the file is properly closed after writing
    } else {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
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
