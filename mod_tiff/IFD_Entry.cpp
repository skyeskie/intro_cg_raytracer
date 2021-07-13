/**
 * IFD_Entry.cpp
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 2
 *
 * Description:
 *         Defines an object to contain all the information for an IFD entry
 *              from a TIFF file
 *         Provides comparison operators (which sort on identifier)
 *              and an insertion operator
 *         No data hiding since it's limited use
 */

#include "IFD_Entry.h"
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

IFD_Entry::IFD_Entry()
    :type(-1), address(0), size(0)
{}

IFD_Entry::IFD_Entry(int type)
    :type(type), address(0), size(0)
{}

bool IFD_Entry::operator<(IFD_Entry& rhs) {
    return (type < rhs.type); //Sort asc by type
}

bool IFD_Entry::operator>(IFD_Entry& rhs) {
    return (type > rhs.type);
}

ostream& operator<<(ostream& out, IFD_Entry& ifde) {
    string types[6] = {"BYTE", "CHAR", "SHORT", "LONG", "RATIONAL", "UNKNOWN"};
    vector<int>::const_iterator it, it2, stop;
    out << ifde.get_name() << " (" << dec << ifde.identifier << ") ";
    out << types[ min(ifde.type-1,5) ] << " (" << ifde.type << ") ";
    out << "x" << ifde.size << " :\t";
    
    switch(ifde.type) {
        case 1: //Byte
        case 3: //Short
        case 4: //Long
            it = ifde.intdata.begin();
            stop = ifde.intdata.end();
            out << "{ ";
            if(it != stop) {
                out << *it;
            }
            while(++it != stop) {
                out << ", " << *it;
            }
            out << " }";
            break;
            
        case 2: //ASCII string
            out << '"' << ifde.chardata << '"';
            break;
        
        case 5: //Rational
            it = ifde.intdata.begin();
            it2 = ifde.intdata2.begin();
            stop = ifde.intdata.end();
            out << "{ ";
            if(it != stop) {
                out << *it << "/" << *it2;
            }
            while(++it != stop) {
                it2++;
                out << ", " << *it << "/" << *it2;
            }
            out << " }";
            break;

        default:
            out << "{ Unknown Type (" << ifde.type << ") }";
            break;
    }
    return out;
}


string IFD_Entry::get_name() {
    return get_name(identifier);
}

string IFD_Entry::get_name(int id) {
    switch(id) {
        case 254:
            return "NewSubfileType";
        case 255:
            return "SubfileType";
        case 256:
            return "ImageWidth";
        case 257:
            return "ImageLength";
        case 258:
            return "BitsPerSample";
        case 259:
            return "Compression";
        case 262:
            return "PhotometricInterpretation";
        case 263:
            return "Threshholding";
        case 264:
            return "CellWidth";
        case 265:
            return "CellLength";
        case 266:
            return "FillOrder";
        case 269:
            return "DocumentName";
        case 270:
            return "ImageDescription";
        case 271:
            return "Make";
        case 272:
            return "Model";
        case 273:
            return "StripOffsets";
        case 274:
            return "Orientation";
        case 277:
            return "SamplesPerPixel";
        case 278:
            return "RowsPerStrip";
        case 279:
            return "StripByteCounts";
        case 280:
            return "MinSampleValue";
        case 281:
            return "MaxSampleValue";
        case 282:
            return "XResolution";
        case 283:
            return "YResolution";
        case 284:
            return "PlanarConfiguration";
        case 285:
            return "PageName";
        case 286:
            return "XPosition";
        case 287:
            return "YPosition";
        case 288:
            return "FreeOffsets";
        case 289:
            return "FreeByteCounts";
        case 290:
            return "GrayResponseUnit";
        case 291:
            return "GrayResponseCurve";
        case 292:
            return "T4Options";
        case 293:
            return "T6Options";
        case 296:
            return "ResolutionUnit";
        case 297:
            return "PageNumber";
        case 301:
            return "TransferFunction";
        case 305:
            return "Software";
        case 306:
            return "DateTime";
        case 315:
            return "Artist";
        case 316:
            return "HostComputer";
        case 317:
            return "Predictor";
        case 318:
            return "WhitePoint";
        case 319:
            return "PrimaryChromaticities";
        case 320:
            return "ColorMap";
        case 321:
            return "HalftoneHints";
        case 322:
            return "TileWidth";
        case 323:
            return "TileLength";
        case 324:
            return "TileOffsets";
        case 325:
            return "TileByteCounts";
        case 332:
            return "InkSet";
        case 333:
            return "InkNames";
        case 334:
            return "NumberOfInks";
        case 336:
            return "DotRange";
        case 337:
            return "TargetPrinter";
        case 338:
            return "ExtraSamples";
        case 339:
            return "SampleFormat";
        case 340:
            return "SMinSampleValue";
        case 341:
            return "SMaxSampleValue";
        case 342:
            return "TransferRange";
        case 512:
            return "JPEGProc";
        case 513:
            return "JPEGInterchangeFormat";
        case 514:
            return "JPEGInterchangeFormatLngth";
        case 515:
            return "JPEGRestartInterval";
        case 517:
            return "PEGLosslessPredictors";
        case 518:
            return "JPEGPointTransforms";
        case 519:
            return "JPEGQTables";
        case 520:
            return "JPEGDCTables";
        case 521:
            return "JPEGACTables";
        case 529:
            return "YCbCrCoefficients";
        case 530:
            return "YCbCrSubSampling";
        case 531:
            return "YCbCrPositioning";
        case 532:
            return "ReferenceBlackWhite";
        case 33432:
            return "Copyright";
        default:
            break;
    }
    string tmp = "UnknownTag (";
    tmp += id;
    tmp += ")";
    //cout << tmp << endl;
    return tmp;
}

