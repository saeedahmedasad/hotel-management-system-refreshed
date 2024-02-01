#include <string>

using namespace std;

// Encryption of Password is handled Here
string encrypt(string password)
{
    string alphabets = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+{}[]|:<>?/.,";
    string encrypted_text = "";
    int key = 5;
    for (int i = 0; i < password.length(); i++)
    {
        int index = alphabets.find(password[i]);
        if (index == -1)
        {
            encrypted_text += password[i];
        }
        else
        {
            encrypted_text += alphabets[(index + key) % alphabets.length()];
        }
    }
    return encrypted_text;
}
// Decryption of Password is handled Here
string decrypt(string encrypted)
{
    string alphabets = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+{}[]|:<>?/.,";
    string decrypted_text = "";
    int key = 5;
    for (int i = 0; i < encrypted.length(); i++)
    {
        int index = alphabets.find(encrypted[i]);
        if (index == -1)
        {
            decrypted_text += encrypted[i];
        }
        else
        {
            decrypted_text += alphabets[(index - key + alphabets.length()) % alphabets.length()];
        }
    }
    return decrypted_text;
}