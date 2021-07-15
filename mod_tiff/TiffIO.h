/**
 * TiffIO.h
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 2
 * 
 * Description:
 *         Provides handler class for reading and writing TIFF
 *         file images.
 */


#ifndef TIFFIO_H
#define TIFFIO_H

#include "../mod_base/CommandHandle.h"
#include "../mod_base/ModuleCommand.h"
#include "IFD_Entry.h"
#include <vector>
#include <map>

class TiffIO : public ModuleCommand
{
public:
    //Preferred Ctor -- registers with Command Processor
    TiffIO(CommandHandle& ch);
    TiffIO(); //Still provide default
    
    //Main catch -- handles commands
    virtual std::string execute(std::vector< std::string > argv);

    std::string reset_tags();
    
    //Help functions for module
    virtual void help(const std::string cmd) const;
    //virtual void help(int cmd_num) const;

public: //Main Functions
    /** Main read function, for TiffRead from CLI
     * @arg <filename>: file to try to open
     * @throws runtime_error if unable to read or process
     */
    std::string read_file(std::string filename);
    
    /** Stat function for CLI command TiffStat
     * displays information and tags for TIFF image
     * @arg <filename> to try to stat
     * @throws runtime_error if unable to open file
     * does NOT throw if tries to read invalid TIFF image
     * outputs error to cout on that case
     */
    std::string tiff_stat(std::string filename);
    
    /**Function for CLI command TiffWrite
     * Writes Tiff to file <filename> from
     * specified sub box from (<x0>,<y0>) to (<xf>,<yf>)
     * @throws runtime_error if unable to write image
     */
    std::string write_file(std::string filename, size_t x0, size_t y0, size_t xf, size_t yf);

    //Started function from project 2 spec
    //Testing purposes only
    //Called through [IOTEST filename] in CLI
    std::string starter(std::string filename);
private:
    CommandHandle* ch; //The command handler we're registered with
    
    //Helper -- determine if machine is little endian
    bool little_endian_machine;

    /**
     * Processes a single ifd_entry
     * @arg ifstream already at start of entry
     * @post ifstream is at end of entry
     * @returns tagdata in <IFD_Entry> object
     */
    IFD_Entry ifd_entry_read(std::ifstream&);
    
    /**
     * Check and swap bytes (if needed)
     * will ONLY swap if (<little_endian_machine> XOR <img_little_endian>)
     */
    void endian_chkswap(short&);
    void endian_chkswap(int&);


    /** Opens a tiff image and verifies it's open
     * @arg valid filename and input file stream <imfile>
     * Tries to open from current dir and ./images/ dir
     * @throws runtime_error if unable to open file
     * @post <imfile> is opened for binary read with pointer at beginning
     */
    void tiff_open(std::ifstream& imfile, std::string filename);
    
    /**Main worker function for tiff_stat
     * Preforms prevalidation and loads all tags
     * @arg ifstream opened in binary mode
     * @throws runtime_error if file not open or invalid TIFF
     * @post tags loaded into TiffIO object
     */
    void tiff_read_tags(std::ifstream& imfile);

    /**Validates TIFF image
     * @pre should have loaded tags unless you really want a fail
     * @post Sets img_colortype on success
     * @throws runtime_error on fail
     */
    void tiff_validate();

    /** Helper for <tiff_validate()>
     * @arg the <id> for the TIFF tag to check
     * @returns true iff tag of <id> exists in in the class <img_tags> map
     */
    bool img_tag_exists(int id);

    //Helper functions to get data from a tag in the img_tags structure
    int get_int_tag(int id, int pos=0);
    std::string get_chr_tag(int id);
    float get_rat_tag(int id, int pos=0);

    /** Loads Image from file
     * @pre <imfile> open, tiffdata loaded
     * @post tiffdata loaded
     * @throws runtime_error
     */
    void tiff_load_imgdata(std::ifstream& imfile);

    /** Puts tiff image onto GL window
     * @pre tiffdata loaded, image data loaded
     * @post image put to array and GLut notified for redraw
     */
    void tiff_draw_window();

    //For bilevel and greyscale (and palette)
    //Essentially for all single channel image types
    std::vector< std::vector<short> > img_data_bw;

    //Triplet structure for storing RGBs
    struct pixel {
        unsigned short r, g, b;
    };
    
    //For RGB triplet images
    std::vector< std::vector<pixel> > img_data_rgb;

    //Helper union
    //Used to access individual bytes to facilitate r/w
    //Also used to swap bytes if endianess mismatch
    union bytesplitter {
        int i;  //4bytes
        short s; //2bytes
        char c[4]; //* 4x1byte
    };
    
    /** Helper function for tag read
     * seeks to position,
     * @return past position
     */
    std::streampos read_goto(std::ifstream&, IFD_Entry&);
    

    //Image properties from TiffStat
    //Used for easy access
    int ifd_start_addr;
    int ifd_num_entries;
    bool img_little_endian; //Whether read image is LE
    int img_magic_no;
    int img_colortype;

    unsigned int img_width;
    unsigned int img_height;
    
    //Tag storage
    std::map<int, IFD_Entry> img_tags;

    //std::vector<pixel> img_colormap;
    
};
enum COLOR_TYPES { TIFF_UNK = -1, TIFF_BILEVEL, TIFF_GREYSC, TIFF_RGB, TIFF_PALETTE };

#endif // TIFFIO_H
