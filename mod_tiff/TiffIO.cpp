/**
 * TiffIO.cpp
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 2
 *
 * Description:
 *         Implements handler class for reading and writing TIFF
 *         file images.
 */

#include "TiffIO.h"
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <math.h>
#include <stdlib.h>
#include "../mod_base/CommandHandle.h"
#include "IFD_Entry.h"
using namespace std;


//enum COLOR_TYPES { BILEVEL_IMG, GREYSC_IMG, PALETTE_IMG, RGB_IMG };

TiffIO::TiffIO(CommandHandle& cho): ModuleCommand() , ch(&cho),
    little_endian_machine(true) //Hardcode for now. Can put test in c'tor
{
    cho.add_command("iotest", *this);
    cho.add_command("tiffstat", *this);
    cho.add_command("tiffread", *this);
    cho.add_command("tiffwrite", *this);
    cho.add_command("tiffreset", *this);

    //Setup default tags
    reset_tags();
}

TiffIO::TiffIO() : ModuleCommand(),
    little_endian_machine(true)//Hardcode for now. Can put test in c'tor
{
    reset_tags();
}

string TiffIO::reset_tags() {
    img_tags.clear();
    typedef std::pair<int, IFD_Entry> TAG;
    IFD_Entry data;
    //NewSubfileType (254) LONG (4) x1 :    { 0 }
    data.type = 4; data.identifier = 254; data.size = 1;
    data.intdata.push_back(0);
    img_tags.insert(TAG(data.identifier,data));

    
    //Compression (259) SHORT (3) x1 :      { 1 }
    data.intdata[0] = 1;
    data.type = 3; data.identifier = 259; data.size = 1;
    img_tags.insert(TAG(data.identifier,data));
    //PlanarConfiguration (284) SHORT (3) x1 :      { 1 }
    data.type = 3; data.identifier = 284; data.size = 1;
    img_tags.insert(TAG(data.identifier,data));
    //Orientation (274) SHORT (3) x1 :      { 1 }
    data.type = 3; data.identifier = 274; data.size = 1;
    img_tags.insert(TAG(data.identifier,data));

    //ResolutionUnit (296) SHORT (3) x1 :   { 2 }
    data.type = 3; data.identifier = 296; data.size = 1;
    data.intdata[0] = 2;
    img_tags.insert(TAG(data.identifier,data));
    //PhotometricInterpretation (262) SHORT (3) x1 :        { 2 }
    data.type = 3; data.identifier = 262; data.size = 1;
    img_tags.insert(TAG(data.identifier,data));
    
    //SamplesPerPixel (277) SHORT (3) x1 :  { 3 }
    data.type = 3; data.identifier = 277; data.size = 1;
    data.intdata[0] = 3;
    img_tags.insert(TAG(data.identifier,data));
    
    data.intdata[0] = 96; data.intdata2.push_back(1);
    //XResolution (282) RATIONAL (5) x1 :   { 96/1 }
    data.type = 5; data.identifier = 282; data.size = 1;
    img_tags.insert(TAG(data.identifier,data));
    //YResolution (283) RATIONAL (5) x1 :   { 96/1 }
    data.type = 5; data.identifier = 283; data.size = 1;
    img_tags.insert(TAG(data.identifier,data));
    
    //BitsPerSample (258) SHORT (3) x3 :    { 8, 8, 8 }
    data.type = 3; data.identifier = 258; data.size = 3;
    data.intdata[0] = 8; data.intdata.push_back(8); data.intdata.push_back(8);
    img_tags.insert(TAG(data.identifier,data));

    //Cue to get from window
    img_width = 0;
    img_height = 0;

    //ImageWidth (256) LONG (4) x1 :        { 509 }
    data.type = 4; data.identifier = 256; data.size = 1;
    data.intdata[0] = img_width;
    img_tags.insert(TAG(data.identifier,data));
    //ImageLength (257) LONG (4) x1 :       { 508 }
    data.type = 4; data.identifier = 257; data.size = 1;
    data.intdata[0] = img_height;
    img_tags.insert(TAG(data.identifier,data));


    //Will be filled in by tiffwrite, but we need to make sure tags exist
    data.type = 4; data.size = 1; data.intdata[0] = 0;
    data.identifier = 273;//StripOffsets
    img_tags.insert(TAG(data.identifier,data));
    data.identifier = 278;//RowsPerStrip
    img_tags.insert(TAG(data.identifier,data));
    data.identifier = 279;//StripByteCounts
    img_tags.insert(TAG(data.identifier,data));

    img_colortype = TIFF_RGB;
    
    return "null";
}


string TiffIO::execute(vector< string > argv) {
    if(argv.size()==0)
        return "Must provide function";
    
    //Split off command
    string cmd = argv[0];
    argv.erase( argv.begin() );
    if(cmd=="tiffreset") return reset_tags();
        
    //For now, hardcode starter function in
    if(argv.size()==1) {
        if(cmd=="tiffstat") return tiff_stat(argv[0]);
        if(cmd=="tiffread") return read_file(argv[0]);
        if(cmd=="iotest") return starter(argv[0]);
    }
    if(cmd=="tiffwrite") {
        //Setup defaults
        
        //Default change size to match window bounds
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        img_width = min(abs(viewport[2]-viewport[0]),MAX_IMG_WIDTH);
        img_height = min(abs(viewport[3]-viewport[1]),MAX_IMG_HEIGHT);

        ostringstream strconv;
        switch(argv.size()) {
            case 0:
                throw invalid_argument("Must specify output filename.");
                break;

            //Default Arguments
            case 1:
                argv.push_back("0");
            case 2:
                argv.push_back("0");
            case 3:
                strconv.str("");
                strconv << img_width-1;
                strconv.flush();
                argv.push_back(strconv.str().c_str());
            case 4:
                strconv.str("");
                strconv << img_height-1;
                strconv.flush();
                argv.push_back(strconv.str().c_str());
            case 5:
                break;

            default:
                throw invalid_argument("Too many arguments for TiffWrite.");
                break;
        }
        //Parse arguments and call function
        cout << "Writing TIFF Image..." << endl;
        //write_file(argv[0], 0, 0, (img_width-1), (img_height-1));
        write_file(argv[0], sanitize_int(argv[1]), sanitize_int(argv[2]),
                    sanitize_int(argv[3]), sanitize_int(argv[4]));
        
        return "null";
    } else throw invalid_argument("Invalid number of arguments for "+cmd);

    return "null";
}

void TiffIO::help(const std::string cmd) const
{
    ModuleCommand::help(cmd);
}

string TiffIO::read_file(string filename) {
    ifstream imfile;
    tiff_open(imfile, filename);
    tiff_read_tags(imfile);
    tiff_validate();
    tiff_load_imgdata(imfile);
    tiff_draw_window();
    imfile.close();
    return "redraw";
}

string TiffIO::tiff_stat(string filename) {
    ifstream imfile;
    tiff_open(imfile, filename);
    //Back up old tags
    map<int, IFD_Entry> old_tags;
    old_tags = img_tags;
    int old_wid = img_width,
        old_len = img_height,
        old_type = img_colortype;
    bool tags_loaded = false;
    
    //tiff_stat is a little more forgiving
    //It won't crash the read stack on fail
    try {
        tiff_read_tags(imfile);
	tags_loaded = true;
	
        cout << "TiffStat on [" << filename << "]:" << endl;
        cout << "Endian: " << ((img_little_endian)?"Little":"Big") << "\n";
        cout << "Magic: " << dec << img_magic_no << "\n";
        cout << "IFD Address: ";
        cout.width(10); cout << hex << showbase << right << ifd_start_addr << endl;
        cout << "IFD Count: " << dec << ifd_num_entries << "\n";

        tiff_validate();
        cout << "Valid TIFF." << endl;
        cout << "Image Type:" << img_colortype << endl;
        cout << "Contents of Directory 0:" << endl;
    }
    catch (runtime_error excpt) {
        if(tags_loaded) {
            cout << "Validation failed: " << excpt.what() << endl;
        } else {
            cout << "Error: " << excpt.what() << endl;
        }
        //Restore old tags
        img_tags  = old_tags;
        img_width = old_wid;
        img_height= old_len;
        img_colortype = old_type;
        return "null";
    }

    
    
    map<int, IFD_Entry>::iterator it;
    for(it = img_tags.begin(); it != img_tags.end(); ++it) {
        cout << "  " <<  it->second << endl;
    }
    cout << endl;
    imfile.close();

    //Restore old tags
    img_tags = old_tags;
    img_width = old_wid;
    img_height= old_len;
    img_colortype = old_type;
    return "null";
}

string TiffIO::write_file(string filename, size_t x0, size_t y0, size_t xf, size_t yf)
{
    //~cout << "Entering write for "+filename << endl;
    //~cout << "\tBox: ("<<x0<<","<<y0<<") to ("<<xf<<","<<yf<<")"<<endl;

    //Make sure actually HAVE something read in
    if(img_tags.size()==0) {
        throw runtime_error("No image to write. Must call TiffRead first.");
    }
    
    //Swap box to usable format if need to
    if(x0 > xf) swap(xf, x0);
    if(y0 > yf) swap(yf, y0);
    
    //Arg checks
    if(x0>=xf || y0>=yf || xf>img_width || yf>img_height) {
        cerr << x0 << ", " << xf << ", " << y0 << ", " << yf
        << " ( " << img_width << ", " << img_height << " )" << endl;
        throw invalid_argument("Box out of image bounds.");
    }
   
    ofstream fp;
    fp.open(filename.c_str(), ios::binary);
    if(!fp.is_open()) {
        throw runtime_error("Unable to open \""+filename+"\" for writing.");
    }
    
    //Update tags to our writable format
    bool rgb = (img_colortype == TIFF_RGB);
    if(img_tag_exists(277)) img_tags[277].intdata[0] = 3; //Force RGB only
    while(img_tags[258].intdata.size()>3) {
        img_tags[258].intdata.pop_back();
        img_tags[258].size = 3;
    }
    if(img_tag_exists(338)) img_tags.erase(338); //Kill ExtraSamples
    img_tags[258].intdata[0] = 8; //Force 8bits per sample
    int img_len = yf - y0 + 1;
    int img_wid = xf - x0 + 1;
    img_tags[256].intdata[0] = img_wid;
    img_tags[257].intdata[0] = img_len;
    //Should probably update resolution too...
    int img_num_bytes = img_wid*img_len;
    if(rgb) img_num_bytes*=3;
    
    //Start writing header
    bytesplitter buffer;
    buffer.c[0] = (little_endian_machine)?'I':'M';
    buffer.c[1] = buffer.c[0];
    fp.write(buffer.c, 2);
    
    buffer.s = 42; //Magic number
    fp.write(buffer.c, 2);

    buffer.i = 8; //Just put IFD right at the beginning
    fp.write(buffer.c, 4);

    buffer.s = (short) img_tags.size(); //Num entries
    fp.write(buffer.c, 2);
    
    int img_offset = 8 + 6 + 12*img_tags.size();
    int aux_offset = img_offset + img_num_bytes;

    //Set strip / row tags
    img_tags[278].intdata.clear(); //RowsPerStrip
    img_tags[278].intdata.push_back(img_len);
    img_tags[278].size = 1;

    img_tags[273].intdata.clear(); //StripOffsets
    img_tags[273].intdata.push_back(img_offset);
    img_tags[273].size = 1;
    
    img_tags[279].intdata.clear(); //StripByteCounts
    img_tags[279].intdata.push_back(img_num_bytes);
    img_tags[279].size = 1;
    
    std::streampos ptrback;
    
    std::map<int, IFD_Entry>::iterator it;

    short sizes[] = { 0, 1, 1, 2, 4, 8 };
    int type_len = 0;
    int n_obj = 0;
    for(it = img_tags.begin(); it!= img_tags.end(); it++) {
        buffer.s = it->second.identifier;
        fp.write(buffer.c, 2); //Write Tag ID
        //~cout << "{ID "<<dec<<buffer.s<<", ";
        buffer.s = it->second.type;
        fp.write(buffer.c, 2); //Write Type
        type_len = sizes[ buffer.s ];
        //~cout << "Type "<<buffer.s<<", ";
        buffer.i = n_obj = it->second.size;
        fp.write(buffer.c, 4); //Write Num data fields in tag
        //~cout << " Count "<<buffer.s<<" Data: ";
        //Determine if we need an offset
        if(type_len*n_obj <= 4) {
            //Write in place
            if(it->second.type == 2) { //We can cheat on chars
                fp.write( it->second.chardata.c_str(), type_len*n_obj);
                //~cout << it->second.chardata;
            } else for(int j=0; j<n_obj; j++) {
                //No need to worry about rational here
                if(type_len==1) {
                    buffer.c[0] = (char) it->second.intdata.at(j);
                    //~cout << hex << buffer.c[0];
                } else if(type_len == 2) {
                    buffer.s = (short) it->second.intdata.at(j);
                    //~cout << hex << buffer.s;
                } else {
                    buffer.i = it->second.intdata.at(j);
                    //~cout << hex << buffer.i;
                }
                fp.write(buffer.c, type_len);
            }
            buffer.i = 0;
            fp.write( buffer.c, 4-type_len*n_obj); //Fill with zeros
            //~cout << ", fill " << (4-type_len*n_obj) << " }" << endl;
            
        } else {
            //We need to jump
            buffer.i = aux_offset;
            fp.write(buffer.c, 4);
            
            //~cout << hex << " JUMP (";
            ptrback = fp.tellp();
            //~cout << ptrback << ") --> " << buffer.i << endl;

            //~cout << "\t{VALS:" << hex;
            
            fp.seekp(aux_offset, ios::beg);
            aux_offset += type_len*n_obj;
            
            if(it->second.type == 2) { //We can cheat on chars
                fp.write( it->second.chardata.c_str(), type_len*n_obj);
                //~cout << it->second.chardata;
            } else for(int j=0; j<n_obj; j++) {
                //~cout << " | ";
                //No need to worry about rational here
                if(type_len==1) {
                    buffer.c[0] = (char) it->second.intdata.at(j);
                    //~cout << buffer.c[0];
                } else if(type_len == 2) {
                    buffer.s = (short) it->second.intdata.at(j);
                    //~cout << buffer.s;
                } else {
                    buffer.i = it->second.intdata.at(0);
                    //~cout << buffer.i;
                }
                fp.write(buffer.c, min(type_len,4));
                if(type_len==8) { //Process rational
                    buffer.i = it->second.intdata2.at(j);
                    fp.write(buffer.c, 4);
                    //~cout << buffer.i;
                }                 
            }
            //~cout << " }" << endl;
            fp.seekp(ptrback);//*/
        }
    }

    //Make sure we write the 4 bytes of zero at the end
    buffer.i = 0;
    fp.write(buffer.c, 4);

    //Write out image data
    //cout <<"init: "<<yf<<">="<<y0<<"("<<img_height<<")"<<endl;
    //cout <<"init: "<<x0<<"<="<<xf<<"("<<img_width<<")"<<endl;

    //~cout << "Beginning image at: " << dec << fp.tellp()<< endl;
    //~cout << "Calc offset is: " << img_offset << endl;
    for(int i=yf; i>=int(y0) && i<int(img_height); i--) {
        for(int j=x0; j<=int(xf); j++) {
            buffer.c[0] = ch->main_window[i][j][0];
            buffer.c[1] = ch->main_window[i][j][1];
            buffer.c[2] = ch->main_window[i][j][2];
            //cout << "("<<i<<","<<j<<")  ";
            fp.write(buffer.c, (rgb)?3:1);
        }
    }
    //~cout << "ending image at: " << dec << fp.tellp() << endl;
    //~cout << "calc size is: " << dec << img_num_bytes << endl;
    //~cout << "aux offset begins at:";
    //~fp.seekp(img_offset + img_num_bytes);
    //~cout << fp.tellp() << endl;
    fp.close();
    return "File written.";
}

void TiffIO::tiff_open(ifstream& imfile, string filename) {
    imfile.open (filename.c_str(), ios::binary );
    
    if(!imfile.is_open()) {
        //Try to open in images/ directory before throwing error
        string tmp = "images/";
        tmp.append(filename);
        imfile.open(tmp.c_str(), ios::binary );
        
        if(!imfile.is_open()) {
            throw runtime_error("Unable to open file \"" + filename + '"');
        }
    }
}

void TiffIO::tiff_read_tags(ifstream& imfile) {
    if(!imfile.is_open()) {
        throw invalid_argument("Input iostream must be open for binary read.");
    }
    imfile.seekg (0, ios::beg); //not strictly necessary, but a good idea
    
    char buffer2[2];
    short buffer_sh;
    int buffer_int;
    //Determine and set endianness for image
    imfile.read(buffer2, 2);
    if(buffer2[0] == 'I' && buffer2[1] =='I') {
            img_little_endian = true;
    } else if(buffer2[0] == 'M' && buffer2[1] =='M') {
            img_little_endian = false;
    } else {
            throw runtime_error("Unexpected value ("+string(buffer2)
                            +") encountered reading file at bytes 0 and 1.");
    }
    
    //Check magic number
    imfile.read((char*) &buffer_sh, 2);
    endian_chkswap(buffer_sh);
    if(buffer_sh != 42) {
            throw runtime_error("Bad magic number in image.");
    }
    img_magic_no = buffer_sh;
    
    //Get first IFD address
    imfile.read ((char*) &buffer_int, 4);
    endian_chkswap(buffer_int);
    imfile.seekg(buffer_int, ios::beg);
    
    //Processing IFD entry
    ifd_start_addr = buffer_int;
    //Seek to and read count
    imfile.seekg(ifd_start_addr, ios::beg);
    imfile.read( (char*) &buffer_sh, 2);
    endian_chkswap(buffer_sh);
    ifd_num_entries = buffer_sh;

    //Clear old info
    img_tags.clear();
    IFD_Entry ifde_temp;
    //Read entries
    for(int i = 0; i < ifd_num_entries; i++) {
        ifde_temp = ifd_entry_read(imfile);
        std::pair<int, IFD_Entry> new_tag(ifde_temp.identifier, ifde_temp);
        img_tags.insert(new_tag);
    }
    
    //Read for next
    imfile.read( (char*) &buffer_int, 4);
    endian_chkswap(buffer_int);
    if(buffer_int != 0) {
        cout << "Notice: Multiple IFDs found in file." << endl;
        cout << "\tOnly first IFD processed. Others ignored." << endl;
    }
    
}

void TiffIO::tiff_validate() {
    IFD_Entry tag; //So we can use the get_name function
    int val;
    img_colortype = TIFF_UNK;
    //Check tags required for all types
    int req_tags[10] = {
        256, //ImageWidth
        257, //ImageLength
        259, //Compression
        262, //PhotometricInterpretation
        273, //StripOffsetts
        278, //RowsPerStrip
        279, //StripByteCounts
        282, //XResolution
        283, //YResolution
        296  //ResolutionUnit
    };
    
    for(int i=0; i<7; i++) {
        if(!img_tag_exists(req_tags[i]))
            throw runtime_error("Missing tag "+tag.get_name(req_tags[i]));
    }

    //Set length and width from tags for easy access
    img_width = get_int_tag(256);
    img_height = get_int_tag(257);

    //val = img_tags[296].intdata.at(0); //Check ResolutionUnit
    //if(val<0 || val>3)
    //    throw runtime_error("Unexpected value in tag "+tag.get_name(296));

    //Determine type
    switch(get_int_tag(262)) {
        case TIFF_BILEVEL:
        case TIFF_GREYSC://Handle after
            break;
            
        case TIFF_PALETTE: //Palette image handling
            img_colortype = TIFF_PALETTE;
            val = get_int_tag(259); //Compression
            if( !(val==1 || val==32773) )
              throw("Unexpected value in "+tag.get_name(259)+" for palette img");

            if(!img_tag_exists(320)) //Make sure ColorMap exists
                throw runtime_error("Missing tag "+tag.get_name(320));

            if(!img_tag_exists(258)) //BitsPerSample
                throw runtime_error("Missing tag "+tag.get_name(258));
            val = get_int_tag(258);
            if( !(val==4 || val==8) )
                throw("Unexpected value in "+tag.get_name(258)+" for palette img");
            return;
            break;
            
        case TIFF_RGB: //RGB handling
            img_colortype = TIFF_RGB;
            val = get_int_tag(259);
            if( !(val==1 || val==32773) ) {
              throw runtime_error("Unexpected value in "+tag.get_name(259)
                                +" for RBG img");
            }
            
            if(!img_tag_exists(258)) { //BitsPerSample
              throw runtime_error("Missing tag "+tag.get_name(258));
            }
            
            if((get_int_tag(258,0)!=8 || get_int_tag(258,1)!=8 || get_int_tag(258,2)!=8) )
            {
              throw runtime_error("Unexpected values in "+tag.get_name(258)
                                +" for RGB img");
            }
            
            if(!img_tag_exists(277)) { //SamplesPerPixel
                throw runtime_error("Missing tag "+tag.get_name(277));
            }
            val = get_int_tag(277);
            if( val<3 ) {
              throw runtime_error("Unexpected value in "+tag.get_name(277)
                                +" for RGB img");
            }

            return;
            break;

        default:
            img_colortype = TIFF_UNK;
            throw runtime_error("Unexpected value in "+tag.get_name(262));

            return;
    }
    
    //Only greyscale and bilevel should reach here
    
    //Set to one if bilevel is missing
    if(!img_tag_exists(258)) {
      IFD_Entry def_tag(3);
      def_tag.identifier = 258;
      def_tag.intdata.push_back(1);
    }
    
    if(get_int_tag(258) == 1) { //Differentiates between bilevel and greyscale
        img_colortype = TIFF_BILEVEL;
        val = get_int_tag(259); //Compression
        if( !(val==1 || val==2 || val==32773) )
            throw runtime_error("Unexpected value in "+tag.get_name(259)+" for bilevel img");
    } else {
        img_colortype = TIFF_GREYSC;
        if(!img_tag_exists(258))
            throw runtime_error("Missing tag "+tag.get_name(258));
        val = get_int_tag(258); //BitsPerSample
        if( !(val==4 || val==8) )
          throw runtime_error("Unexpected value in "+tag.get_name(258)+" for greyscale img");
        val = get_int_tag(259); //Compression
        if( !(val==1 || val==32773) )
          throw runtime_error ("Unexpected value in "+tag.get_name(259)+" for greyscale img");
    }
}

void TiffIO::tiff_load_imgdata(ifstream& imfile) {
    //Hardcore max
    if(!img_width || !img_height)
        throw runtime_error("Image must be loaded and >0 in both dimensions.");
    if(img_width > 1024 || img_height > 1024)
        throw runtime_error("Image larger than max size of 1024x1024");
    if(get_int_tag(259)!=1) //Compression
        throw runtime_error("Only uncompressed TIFF images supported.");

    img_data_bw.clear();
    img_data_rgb.clear();
    
    if(img_colortype == TIFF_GREYSC) {
        img_data_bw.resize(img_height);
        char buffer;

        //Figure out mult. factor to get highest value to 255
        float multby;
        if(img_colortype == TIFF_BILEVEL) multby = 255;
        else multby = (get_int_tag(258)==4)? 15.9375 :1; //Check BitsPerSample

        vector<int>* strips = &(img_tags[273].intdata);
        int rows_strip = get_int_tag(278);

        int y_c = 0;
        //Loop over all strips
        for(uint s=0; s< strips->size(); s++) {
            imfile.seekg(strips->at(s), ios::beg);
            //Loop over all rows in strip
            for(int i=0; i< rows_strip && y_c < (int)img_height; i++, y_c++) {
                img_data_bw[y_c].resize(img_width);
                //Read in a row
                for(uint j=0; j<img_width; j++) {
                    imfile.read(&buffer, 1); //One byte wide on Bilevel
                    buffer = int( float(buffer) * multby ); //Scale
                    //Do we need to invert the image? (check PhotoInterpt)
                    if(!get_int_tag(262)) buffer = 255 - buffer;
                    img_data_bw[y_c][j] = (short) buffer;
                }
            }
        }
    } else if(img_colortype == TIFF_RGB) {
        pixel buffer;
        img_data_rgb.resize(img_height);

        //Determine how much to throwaway if extra samples
        int throwaway = 0;
        if(img_tags[258].intdata.size() > 3) {
            vector<int>::iterator it = img_tags[258].intdata.begin() + 2;
            while(++it != img_tags[258].intdata.end()) {
                throwaway+= *it; 
            }
        }
        
        vector<int>* strips = &(img_tags[273].intdata);
        int rows_strip = get_int_tag(278);
        
        int y_c = 0;
        
        //Loop over all strips
        for(uint s=0; s< strips->size(); s++) {
            imfile.seekg(strips->at(s), ios::beg);
            //Loop over all rows in strip
            for(int i=0; i< rows_strip && y_c < (int) img_height; i++, y_c++) {
                img_data_rgb[y_c].resize(img_width);
                //Read in a row
                for(uint j=0; j<img_width; j++) {
                    imfile.read((char*) &(buffer.r), 1);
                    imfile.read((char*) &(buffer.g), 1);
                    imfile.read((char*) &(buffer.b), 1);
                    imfile.seekg((throwaway+7)/8, ios::cur); //Change to bytes
                    img_data_rgb[y_c][j] = buffer;
                }
            }
        }        
    } else if(img_colortype == TIFF_PALETTE) {
        throw runtime_error("Palette images not supported.");
        /*//Load colormap
        vector<int>* raw_colormap = &( img_tags[320].intdata );
        vector<int>::iterator it = raw_colormap->begin();

        pixel tmp;
        bytesplitter pairing;
        int total = raw_colormap->size();
        int per_channel = total/3;

        
        for(int j=0; j<total; j++) {
            if(j%4 == 0) pairing.i = *(it++);
            
            if(j/per_channel==0) {
                //cout << "r";
                tmp.r = pairing.c[j%4];
                img_colormap.push_back(tmp);
            } else if(j/per_channel == 2) {
                //cout << "g";
                img_colormap[j%per_channel].g = pairing.c[j%4];
            } else {
                //cout << "b";
                img_colormap[j%per_channel].b = pairing.c[j%4];
            }
        }
        
        //Load image data
        img_data_bw.resize(img_height);
        char buffer;
        
        vector<int>* strips = &(img_tags[273].intdata);
        int rows_strip = get_int_tag(278);
        
        int y_c = 0;
        //Loop over all strips
        for(uint s=0; s< strips->size(); s++) {
            imfile.seekg(strips->at(s), ios::beg);
            //Loop over all rows in strip
            for(int i=0; i< rows_strip && y_c < (int) img_height; i++, y_c++) {
                img_data_bw[y_c].resize(img_width);
                //Read in a row
                for(uint j=0; j<img_width; j++) {
                    imfile.read(&buffer, 1); //One byte wide
                    img_data_bw[y_c][j] = buffer;
                }
            }
        }//*/
    } else if(img_colortype == TIFF_BILEVEL) {
        throw runtime_error("Bilevel images not supported.");
    } else throw runtime_error("Invalid image type or image not loaded.");
}

void TiffIO::tiff_draw_window() {
    cout << "Drawing image... " << endl;
    

    if(img_colortype == TIFF_BILEVEL || img_colortype == TIFF_GREYSC) {
        for(uint i=0; i<img_height; i++) {
            for(uint j=0; j<img_width; j++) {
                GLubyte tmp = (GLubyte) img_data_bw[img_height-i-1][j];
                //Need to make image show right way
                ch->main_window[i][j][0] = tmp;
                ch->main_window[i][j][1] = tmp;
                ch->main_window[i][j][2] = tmp;
            }
        }
    } else if( img_colortype == TIFF_PALETTE) {
        throw runtime_error("Palette images not supported.");
        /*uint ind;
        pixel tmp;
        for(uint i=0; i<img_height; i++) {
            for(uint j=0; j<img_width; j++) {
                ind = img_data_bw[img_height-i-1][j];
                if(ind>img_colormap.size()) {
                    ch->main_window[i][j][0] = 0;
                    ch->main_window[i][j][0] = 255;
                    ch->main_window[i][j][0] = 0;
                } else {
                    tmp = img_colormap.at(ind);
                    //Need to make image show right way
                    ch->main_window[i][j][0] = (GLubyte) tmp.r;
                    ch->main_window[i][j][1] = (GLubyte) tmp.g;
                    ch->main_window[i][j][2] = (GLubyte) tmp.b;
                }
            }
        }//*/
    } else if(img_colortype == TIFF_RGB) {
        pixel tmp;
        for(uint i=0; i<img_height; i++) {
            for(uint j=0; j<img_width; j++) {
                tmp = img_data_rgb[img_height-i-1][j];
                //Need to make image show right way
                ch->main_window[i][j][0] = (GLubyte) tmp.r;
                ch->main_window[i][j][1] = (GLubyte) tmp.g;
                ch->main_window[i][j][2] = (GLubyte) tmp.b;
            }
        }
    }

    ch->window_height = img_height;
    ch->window_width = img_width;

    //So it will have a redraw even if called from file
    glutReshapeWindow(img_width, img_height);
    glutPostRedisplay();

    return;
}


bool TiffIO::img_tag_exists(int id) {
    return (img_tags.find(id) != img_tags.end());
}

string TiffIO::get_chr_tag(int id)
{
    if(img_tags[id].type != 2 )
        throw runtime_error("Invalid tag type (in char).");
    return img_tags[id].chardata;
}

int TiffIO::get_int_tag(int id, int pos)
{
    int type = img_tags[id].type;
    if(type<=0 || type>5 || type==2)
        throw runtime_error("Invalid tag type");
    return img_tags[id].intdata.at(pos);
}

float TiffIO::get_rat_tag(int id, int pos) {
    if(img_tags[id].type != 5 ) throw runtime_error("Invalid tag type.");
    float n = img_tags[id].intdata.at(pos);
    n = n / img_tags[id].intdata2.at(pos);
    return n;
}



IFD_Entry TiffIO::ifd_entry_read(ifstream& imfile) {
    short buffer_sh, idnum;
    int buffer_int;
    char buffer_ch;
    
    imfile.read( (char*) &idnum, 2); //Tag identifier
    imfile.read( (char*) &buffer_sh, 2); //Field type
    imfile.read( (char*) &buffer_int, 4); //Item count
    endian_chkswap(buffer_sh);
    endian_chkswap(idnum);
    endian_chkswap(buffer_int);
    
    IFD_Entry entry(buffer_sh);
    entry.identifier = idnum;
    entry.size = buffer_int;

    //s~cout << "\tRead:";
    
    std::streampos ptr_back;
    switch( buffer_sh ) { //Switch TYPE
        case 1: //Byte
            if(entry.size > 4) {
                //k~imfile.read( (char*) &buffer_int, 4); break;
                ptr_back = read_goto(imfile, entry);
            }
            for(int i=0; i<entry.size; i++) {
                //s~cout << "1 ";
                imfile.read( &buffer_ch, 1);
                entry.intdata.push_back((int) buffer_ch);
            }
            //Advance pointer
            if(entry.size < 4) {
                imfile.read( (char*) &buffer_int, 4 - entry.size );
            }
            break;
            
        case 2: //ASCII
            if(entry.size > 4) {
                //k~imfile.read( (char*) &buffer_int, 4); break;
                ptr_back = read_goto(imfile, entry);
            }
            for(int i=0; i<entry.size; i++) {
                //s~cout << "1 ";
                imfile.read( &buffer_ch, 1);
                entry.chardata.append(&buffer_ch);
            }
            //Advance pointer
            if(entry.size < 4) {
                //s~cout << "adv" << 4 - entry.size << " ";
                imfile.read( (char*) &buffer_int, 4 - entry.size );
            }
            break;
            
        case 3: //Short
            if(entry.size > 2) {
                //k~imfile.read( (char*) &buffer_int, 4); break;
                ptr_back = read_goto(imfile, entry);
            }
            for(int i=0; i<entry.size; i++) {
                //s~cout << "2 ";
                imfile.read( (char*) &buffer_sh, 2);
                endian_chkswap(buffer_sh);
                entry.intdata.push_back((int) buffer_sh);
            }
            //Advance pointer
            if(entry.size < 2) {
                //s~cout << "adv" << 4 - 2*entry.size << " ";
                imfile.read( (char*) &buffer_int, 4 - 2*entry.size );
            }
            break;
            
        case 4: //Long
            if(entry.size > 1) {
                //k~imfile.read( (char*) &buffer_int, 4); break;
                ptr_back = read_goto(imfile, entry);
            }
            for(int i=0; i<entry.size; i++) {
                //s~cout << "4 ";
                imfile.read( (char*) &buffer_int, 4);
                endian_chkswap(buffer_int);
                entry.intdata.push_back(buffer_int);
            }
            if(!entry.size) {
                //s~cout << "adv4 ";
                imfile.read( (char*) &buffer_int, 4 );
            }
            break;
            
        case 5: //Rational
            //k~imfile.read( (char*) &buffer_int, 4); break;
            ptr_back = read_goto(imfile, entry);
            for(int i=0; i<entry.size; i++) {
                //s~cout << "8 ";
                imfile.read( (char*) &buffer_int, 4);
                endian_chkswap(buffer_int);
                entry.intdata.push_back(buffer_int);
                imfile.read( (char*) &buffer_int, 4);
                endian_chkswap(buffer_int);
                entry.intdata2.push_back(buffer_int);
            }
            break;
            
        default:
            //Unknown type -- can't do anything with it
            //Advance read pointer and exit
            imfile.read( (char*) &buffer_int, 4);
            entry.type = -1;
            return entry;
    }
    
    if(entry.address!=0) {
        //s~cout << "Jump back (" << hex << ptr_back << ")" << endl;
        imfile.seekg(ptr_back);
    }
    
    return entry;
}

std::streampos TiffIO::read_goto(ifstream& imfile, IFD_Entry& entry) {
    int buffer;
    imfile.read( (char*) &buffer, 4);
    endian_chkswap(buffer);
    entry.address = buffer;
    std::streampos ptr = imfile.tellg();
    imfile.seekg(entry.address);
    //s~cout << "Jump (" << hex << ptr << ") -> (" << entry.address << ")\nRead: ";
    return ptr;
}

string TiffIO::starter(string filename) {
    char buffer[3];
    short magicno; // 2 byte quantity
    int ifdaddress; // 4 byte quantity
    short ifdcount;
    // 2 byte quantity
    ifstream imfile;
    imfile.open (filename.c_str(), ios::binary );
    imfile.seekg (0, ios::beg); //not strictly necessary, but a good idea
    if(!imfile.is_open()) {
        throw runtime_error("Unable to open file \"" + filename + '"');
    }
    imfile.read (buffer,2);
    imfile.read ((char *)&magicno,2); //MAKE SURE YOU UNDERSTAND WHY THIS WORKS!
    imfile.read ((char *)&ifdaddress,4);
    imfile.seekg(ifdaddress,ios::beg);
    imfile.read((char *)&ifdcount,2);
    imfile.close();
    buffer[2]='\0'; //Necessary because buffer is a C-style string
    cout << "Endian: " << buffer << "\n";
    cout << "Magic: " << magicno << "\n";
    cout << "IFD Address: " << ifdaddress << "\n";
    cout << "IFD Count: " << ifdcount << "\n";
    return "null";
}

void TiffIO::endian_chkswap(short int& num) {
    //Check
    if(little_endian_machine == img_little_endian) return;

    bytesplitter tmp;
    tmp.s = num;
    swap( tmp.c[0], tmp.c[1] );
    num = tmp.s;
}

void TiffIO::endian_chkswap(int& num) {
    //Check
    if(little_endian_machine == img_little_endian) return;
    
    bytesplitter tmp;
    tmp.i = num;
    //DEBUG
    //cout << "\tendcs debug start:\t" << hex << tmp.i << endl;
    //Swap if needed
    char by;
    
    by = tmp.c[2];
    tmp.c[2] = tmp.c[1];
    tmp.c[1] = by;
    //cout << "\tmid tmp:\t\t" << hex << tmp.i << endl;
    
    by = tmp.c[3];
    tmp.c[3] = tmp.c[0];
    tmp.c[0] = by;
    num = tmp.i;
    //cout << "\tendcs debug fin:\t" << hex << num << endl;
}


