#include "pararchive.hpp"
#include "file_handle.hpp"
#include "parpacket.hpp"
#include "console.hpp"

#include <sstream>
#include <openssl/md5.h>

using std::string;
using std::stringstream;

#define CMP_MD5(a,b) (!memcmp((a), (b), sizeof(md5)))

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

    while(fread(&header, sizeof(header), 1, file->fh) && !is_corrupt) {
        Uint32 packet_size = 0;

        if(header.length > sizeof(header)) {
            packet_size = header.length - sizeof(header);
            packet_body = (char *)malloc(packet_size);
            memset(packet_body, 0, packet_size);
        } else { 
            is_corrupt = 1;
//            console->log("Corrupted out :/");
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
//                console->log("MD5 failure :/");
            }
        }
        if(!is_corrupt) { 

            if(0 == memcmp(header.magic_sequence, magic_packet_sequence, 8)) {
    //            console->log("Magic sequence matches - this is a PAR packet!");
            } else {
                console->log("Not a PAR packet??");
                is_corrupt = 1;
            }
        }
        if(!is_corrupt) { 
            if(0 == memcmp(header.type, main_packet_sequence, 16)) {
//                console->log("Main sequence matches - this is a Main PAR packet!");
            }

            if(0 == memcmp(header.type, filedesc_packet_sequence, 16)) {
    //            console->log("fileDesc sequence matches - this is a file description PAR packet!");
                Uint32 filename_length = header.length - sizeof(header) - 56;
/*                s << "filename length: " << filename_length;
                console->log(s.str());
                s.str("");
  */
                char *name = (char *)malloc(filename_length +1);
                memcpy(name, packet_body + 56, filename_length);
                name[filename_length] = '\0';
                add_parfile(name);
                free(name);
            }
        }
        free(packet_body);
    }

    file->close();
    close_finished_files();

/*
    if(is_corrupt) {
        console->log("parfile is corrupt");
    }else {
        console->log("parfile is NOT corrupt");
    }
    */

}

ParArchive::~ParArchive()
{
}

void ParArchive::add_parfile(string filename)
{
    par_filenames.push_back(filename);
}

Uint32 ParArchive::exists_in_par(string filename)
{
}

Uint32 ParArchive::file_is_corrupt(string filename)
{
}

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
    return !is_recovery_par(filename);
}

int is_recovery_par(string filename)
{
    size_t index = filename.find(".vol");
    return index != string::npos;
}

