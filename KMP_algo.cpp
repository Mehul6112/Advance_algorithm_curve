#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function to build the LPS (Longest Prefix Suffix) array for the pattern
void buildLPSArray(const string& pattern, vector<int>& lps) {
    int m = pattern.length();
    int length = 0; // length of the previous longest prefix suffix
    lps[0] = 0; // LPS[0] is always 0
    int i = 1;

    // Compute LPS array values for pattern[1...m-1]
    while (i < m) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1]; // Try the previous longest prefix suffix
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP pattern searching algorithm
void KMPSearch(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();
    vector<int> lps(m); // LPS array to hold longest prefix suffix values for the pattern

    // Build the LPS array for the pattern
    buildLPSArray(pattern, lps);

    int i = 0; // index for text[]
    int j = 0; // index for pattern[]

    // Loop through the text to find pattern
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        // If we have found the complete pattern
        if (j == m) {
            cout << "Pattern found at index " << i - j << endl;
            j = lps[j - 1]; // Get the next position to check in pattern
        }
        // Mismatch after j matches
        else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1]; // Skip the first matched part of the pattern
            } else {
                i++;
            }
        }
    }
}

// Main function to demonstrate the KMP algorithm
int main() {
    string text, pattern;

    // Input text and pattern
    cout << "Enter the text: ";
    getline(cin, text);
    cout << "Enter the pattern to search for: ";
    getline(cin, pattern);

    // Output the input text and pattern
    cout << "\nText: " << text << endl;
    cout << "Pattern: " << pattern << endl;

    // Call KMPSearch to search for the pattern in the text
    KMPSearch(text, pattern);

    return 0;
}
