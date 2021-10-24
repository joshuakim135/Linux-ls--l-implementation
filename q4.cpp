#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc > 2) {
        cerr << "Too many arguments\n";
    } else if (argc == 2) {
        // if it starts with '/' it is absolute if not then it is relative
        if (argv[1][0] == '/') { // absolute
            string path = argv[1];
            DIR *dp = opendir(path.c_str());
            if (dp == NULL) {
                cout << "Invalid directory" << path << endl;
                return 0;
            }
            vector<string> filenames;
            struct dirent *dirp = readdir(dp);
            while (dirp != NULL) {
                if (dirp->d_name[0] != '.') {
                    filenames.push_back(dirp->d_name);
                }
                dirp = readdir(dp);
                sort(filenames.begin(), filenames.end());
            }
            for (int i = 0; i < filenames.size(); i++) {
                cout << filenames[i] << "  ";
            }
            cout << endl;
            closedir(dp);
        } else {
            char buffer[260];
            string path = getcwd(buffer, 260);
            // parse given argument
            path += "/";
            path += argv[1];
            DIR *dp = opendir(path.c_str());
            if (dp == NULL) {
                cout << "Invalid directory" << path << endl;
                return 0;
            }
            vector<string> filenames;
            struct dirent *dirp = readdir(dp);
            while (dirp != NULL) {
                if (dirp->d_name[0] != '.') {
                    filenames.push_back(dirp->d_name);
                }
                dirp = readdir(dp);
                sort(filenames.begin(), filenames.end());
            }
            for (int i = 0; i < filenames.size(); i++) {
                cout << filenames[i] << "  ";
            }
            cout << endl;
            closedir(dp);
        }
    } else {
        // default to "ls-l" command
        char buffer[260];
        string path = getcwd(buffer, 260);
        DIR *dp = opendir(path.c_str());
        if (dp == NULL) {
            cout << "Invalid directory" << path << endl;
            return 0;
        }
        vector<string> filenames;
        struct dirent *dirp = readdir(dp);
        while (dirp != NULL) {
            if (dirp->d_name[0] != '.') {
                filenames.push_back(dirp->d_name);
            }
            dirp = readdir(dp);
            sort(filenames.begin(), filenames.end());
        }

        // loop through the filenames and get stats
        for (string filename : filenames) {
            string pathName = path + "/" + filename;
            struct stat fileStat;
            struct dirent *file;

            if (stat(pathName.c_str(), &fileStat) < 0) {
                fprintf(stderr, "Cannot read file %s : %s\n", pathName.c_str(), strerror(errno));
                return errno;
            }

            // get permissionsssssss
            printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

            // get number of hard links
            printf(" %lu ", fileStat.st_nlink);
            
            // get owner
            struct passwd *tf;
            tf = getpwuid(fileStat.st_uid);
            printf("%s ", tf->pw_name);

            // get group
            struct group *gf;
            gf = getgrgid(fileStat.st_gid);
            printf("%s ", gf->gr_name);
            
            // file size
            printf("%zu ", fileStat.st_size);
            
            // time of last modification
            char* lastModifiedTime = ctime(&fileStat.st_mtime);
            for (int i = 4; i <= 15; i++) {
                printf("%c", lastModifiedTime[i]);
            }
            
            // filename
            printf(" %s\n", filename.c_str());
        }
    }

    return 0;
}