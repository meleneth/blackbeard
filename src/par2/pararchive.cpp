#include "pararchive.hpp"
#include "file_handle.hpp"
#include "parpacket.hpp"
#include "console.hpp"
#include "md5_stuff.hpp"

#include <sstream>
#include <stdlib.h>
#include <string.h>

using std::string;
using std::stringstream;

ParArchive::ParArchive(string filename)
{
    is_corrupt = 0;
    par_header header;
    char *packet_body;
    unsigned char digest[16];
    MD5_CTX context;
    FileHandle *file = open_filehandle(filename);
    stringstream s;
    const char *magic_packet_sequence = "PAR2\0PKT";
    const char *main_packet_sequence = "PAR 2.0\0Main\0\0\0\0"; 
    const char *filedesc_packet_sequence = "PAR 2.0\0FileDesc";
    console->log("par2 parsing: " + filename);

    while(fread(&header, sizeof(header), 1, file->fh) && !is_corrupt) {
        Uint32 packet_size = 0;

        if(header.length > sizeof(header) && (header.length < 1000000)) {
            packet_size = header.length - sizeof(header);
            packet_body = (char *)malloc(packet_size);
        } else { 
            is_corrupt = 1;
            packet_body = (char *)malloc(1);
        }

        if(!is_corrupt) { 
            if(1 != fread(packet_body, packet_size, 1, file->fh)){
                console->log("Short read on body.  I have no choice but to turn thee in for a corrupt file!");
                is_corrupt = 1;
            }
        }

        if(!is_corrupt) { 
            MD5_Init(&context);
            MD5_Update(&context, header.recovery_set_id, 32);
            MD5_Update(&context, packet_body, packet_size);
            MD5_Final(digest, &context);

            if(!CMP_MD5(digest, header.packet_hash)){
                is_corrupt = 1;
            }
        }
        if(!is_corrupt) { 
            if(0 == memcmp(header.magic_sequence, magic_packet_sequence, 8)) {
            } else {
                console->log("Not a PAR packet??");
                is_corrupt = 1;
            }
        }
        if(!is_corrupt) { 
            if(0 == memcmp(header.type, main_packet_sequence, 16)) {
            }

            if(0 == memcmp(header.type, filedesc_packet_sequence, 16)) {
                Uint32 filename_length = header.length - sizeof(header) - 56;
                char *name = (char *)malloc(filename_length +1);
                memcpy(name, packet_body + 56, filename_length);
                name[filename_length] = '\0';
                ParFileInfo *new_file = new ParFileInfo(name);
                memcpy(new_file->hash, packet_body + 16, 16);
//                console->log("File " + new_file->filename);
//                console->log(" hash: " + hash_to_hex(new_file->hash));
                par_files.push_back(new_file);
                free(name);
            }
        }
        free(packet_body);
    }

    file->close();
    close_finished_files();

}

ParArchive::~ParArchive()
{
    vector<ParFileInfo *>::iterator i;
    for(i=par_files.begin(); i!=par_files.end(); ++i){
        delete *i;
    }
}

Uint32 ParArchive::exists_in_par(string filename)
{
    return false; // FIXME
}

Uint32 ParArchive::file_is_corrupt(string filename)
{
    return false; // FIXME
}

//----------------------------------------------------------------------------------------------

ParFileInfo::ParFileInfo(string filename)
{
    this->filename = filename;
}

ParFileInfo::~ParFileInfo()
{
}

//----------------------------------------------------------------------------------------------

ParArchive *load_par_file(string filename)
{
    ParArchive *archive = new ParArchive(filename);
    return archive;
}

int is_par(string filename)
{
    if(filename.length() < 5) 
        return 0;
    string check = filename.substr(filename.size() - 5, 5);
    if(0 == check.compare(".par2")) return 1;
    if(0 == check.compare(".PAR2")) return 1;
    return 0;
}

int is_base_par(string filename)
{
    return is_par(filename) && !is_recovery_par(filename);
}

int is_recovery_par(string filename)
{
    size_t index = filename.find(".vol");
    return is_par(filename) && index != string::npos;
}

string base_par_filename(string filename)
{
    string mangled = filename.substr(0, filename.length() - 5);
    string lil_vol = ".vol";
    size_t index = filename.find(lil_vol);
    if(index!=string::npos){
        mangled = mangled.substr(0, index);
    }

    string big_vol = ".VOL";
    index = filename.find(big_vol);
    if(index!=string::npos){
        mangled = mangled.substr(0, index);
    }
    return mangled;
}

