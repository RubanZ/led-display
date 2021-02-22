#include "FileSystem.h"

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    ESP_LOGW('FileSystem', "Listing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if(!root){
        ESP_LOGE('FileSystem', "Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        ESP_LOGW('FileSystem', "Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.print (file.name());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.print(file.size());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    ESP_LOGI('FileSystem', "Creating Dir: %s", path);
    if(fs.mkdir(path)) ESP_LOGI('FileSystem', "Dir created");
    else ESP_LOGE('FileSystem', "rmdir failed");
}

void removeDir(fs::FS &fs, const char * path){
    ESP_LOGI('FileSystem', "Removing Dir: %s", path);
    if(fs.rmdir(path)) ESP_LOGI('FileSystem', "Dir removed");
    else ESP_LOGE('FileSystem', "rmdir failed");
}

void readFile(fs::FS &fs, const char * path){
    ESP_LOGI('FileSystem', "Reading file: %s", path);

    File file = fs.open(path);
    if(!file){
        ESP_LOGE('FileSystem', "Failed to open file for reading");
        return;
    }

    while(file.available()) Serial.write(file.read());
    file.close();
}



void writeFile(fs::FS &fs, const char * path, const char * message){
    ESP_LOGI('FileSystem', "Writing file: %s", path);
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        ESP_LOGE('FileSystem', "Failed to open file for writing");
        return;
    }
    if(file.print(message)) ESP_LOGI('FileSystem', "File written");
    else ESP_LOGE('FileSystem', "Write failed");
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    ESP_LOGI('FileSystem', "Appending to file: %s", path);
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        ESP_LOGE('FileSystem', "Failed to open file for appending");
        return;
    }
    if(file.print(message)) ESP_LOGI('FileSystem', "Message appended");
    else ESP_LOGE('FileSystem', "Append failed");

    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    ESP_LOGI('FileSystem', "Renaming file %s to %s", path1, path2);
    if (fs.rename(path1, path2)) ESP_LOGI('FileSystem', "File renamed");
    else ESP_LOGE('FileSystem', "File failed");
}

void deleteFile(fs::FS &fs, const char * path){
    ESP_LOGI('FileSystem', "Deleting file: %s", path);
    if(fs.remove(path)) ESP_LOGI('FileSystem', "File deleted");
    else ESP_LOGE('FileSystem', "File failed");
}

void initFileSystem(){
    if(!SPIFFS.begin(true)){
        ESP_LOGE('FileSystem', "FFat Mount Failed");
        return;
    }
}