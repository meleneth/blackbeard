#include "pararchive.hpp"
#include "file_handle.hpp"
#include "parpacket.hpp"
#include "console.hpp"
#include <sstream>

using std::string;
using std::stringstream;

ParArchive::ParArchive(string filename)
{
    par_header header;
    char *packet_body;
    FileHandle *file = open_filehandle(filename);
    console->log("Opened PAR2 file: " + filename);
    fread(&header, sizeof(header), 1, file->fh);
    stringstream s;

    const char *magic_packet_sequence = "PAR2\0PKT";
    if(0 == memcmp(header.magic_sequence, magic_packet_sequence, 8)) {
        console->log("Magic sequence matches - this is a PAR packet!");
    }

    const char *main_packet_sequence = "PAR 2.0\0Main\0\0\0\0"; 
    if(0 == memcmp(header.type, main_packet_sequence, 16)) {
        console->log("Main sequence matches - this is a Main PAR packet!");
    }

    const char *filedesc_packet_sequence = "PAR 2.0\0FileDesc";
    if(0 == memcmp(header.type, filedesc_packet_sequence, 16)) {
        console->log("fileDesc sequence matches - this is a file description PAR packet!");
        Uint32 packet_size = header.length - sizeof(header);
        packet_body = (char *)malloc(packet_size);
        fread(packet_body, packet_size, 1, file->fh);
        par_filedesc_packet *body = (par_filedesc_packet *)packet_body;
        s << "FileLength is: " << body->length;
        console->log(s.str());
        s.str("");

        Uint32 filename_length = header.length - sizeof(header) - sizeof(body);
        s << "filename length: " << filename_length;
        console->log(s.str());
        s.str("");
        char *name = (char *)malloc(filename_length +1);
        memcpy(name, packet_body + 56, filename_length);
        console->log(name);
        console->log("Well that was the name");
        free(name);
    }


    file->close();
    close_finished_files();

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

Uint32 ParArchive::is_corrupt(string filename)
{
}

ParArchive *load_par_file(string filename)
{
    ParArchive *archive = new ParArchive(filename);
    return archive;
}
