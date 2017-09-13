#ifndef SIMPLE_VTK_HPP_INCLUDED
#define SIMPLE_VTK_HPP_INCLUDED

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#ifdef USE_BOOST
#include <functional>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#endif

class SimpleVTK {
    private:
        const std::string header = R"(<?xml version="1.0" ?>
)";
        std::string content;
        std::string buffer;
        std::string newLine;
        bool endEdit = false;
        unsigned int innerElementPerLine = 10;
        std::string current_vtk_type;


        // indent management
        std::string indent;
        unsigned int currentIndentation = -1;
        void addIndent() {
            ++currentIndentation;
        }

        void backIndent() {
            if (currentIndentation > 0) --currentIndentation;
        }

        void insertIndent() {
            for(unsigned int i = 0; i < currentIndentation; ++i) {
                buffer += indent;
            }
        }


        // store current processing tag information for type validation
        enum class TAG {
            VTKFile,
            ImageData, RectlinearGrid, StructuredGrid, PolyData, UnstructuredGrid,
            PImageData, PRectlinearGrid, PStructuredGrid, PPolyData, PUnstructuredGrid,
            Piece, PointData, CellData, Points, Cells, Coordinates, Verts, Polys, Strips, Lines,
            PPointData, PCellData, PPoints, PCells, PCoordinates,
            DataArray, PDataArray,
            Inner, Unknown
        };
        TAG current_tag;

        void setCurrentTag(const std::string& tag) {
            if (tag == "VTKFile") {
                current_tag = TAG::VTKFile;
            } else if (tag == "DataArray") {
                current_tag = TAG::DataArray;
            } else if (tag == "PDataArray") {
                current_tag = TAG::PDataArray;
            } else if (tag == "Inner") {
                current_tag = TAG::Inner;
            } else {
                current_tag = TAG::Unknown;
            }
        }


        // Initialize Current State
        void initialize() {
            setNewLineCodeLF();
            setIndentSpaceSize();
            endEdit = false;
            content = header;
            current_vtk_type = "";
        }


        // Structure Management
        void beginElement(const std::string& tag) {
            if (buffer != "") {
                commitBuffer();
            }

            addIndent();
            insertIndent();
            setCurrentTag(tag);

            buffer += "<" + getCurrentTagString();
        }

        void endElement(const std::string& tag) {
            if (buffer != "") {
                commitBuffer();
            }

            insertIndent();
            buffer += "</" + convertTagString(tag);
            commitBuffer();

            backIndent();
        }

        void commitBuffer() {
            content += buffer + ">" + newLine;
            buffer.clear();
        }

        void endOneLineElement(const std::string& tag) {
            if (buffer != "") {
                commitOneLineBuffer();
            }
            backIndent();
        }

        void commitOneLineBuffer() {
            content += buffer + "/>" + newLine;
            buffer.clear();
        }

        void beginInnerElement() {
            if (buffer != "") {
                commitBuffer();
            }
            addIndent();
            insertIndent();

            setCurrentTag("Inner");
        }

        void endInnerElement() {
            if (buffer != "") {
                commitInnerBuffer();
            }
            backIndent();
        }

        void commitInnerBuffer() {
            content += buffer;
            buffer.clear();
        }


        // Type checking
        static constexpr int VTKFile_type_length = 12;
        std::array<std::string, VTKFile_type_length> VTKFileType {{
            "ImageData", "RectlinearGrid", "StructuredGrid", "PolyData", "UnstructuredGrid", "vtkHierarchicalBoxDataSet",
            "PImageData", "PRectlinearGrid", "PStructuredGrid", "PPolyData", "PUnstructuredGrid", "PvtkHierarchicalBoxDataSet",
        }};

        static constexpr int format_type_length = 3;
        std::array<std::string, format_type_length> FormatType {{"ascii", "binary", "appended"}};

        static constexpr int byte_order_type_length = 2;
        std::array<std::string, byte_order_type_length> ByteOrderType {{"LittleEndian", "BigEndian"}};

        static constexpr int number_type_length = 10;
        std::array<std::string, number_type_length> NumberType {{
            "Float32", "Float64",
            "Int8", "Int16", "Int32", "Int64",
            "UInt8", "UInt16", "UInt32", "UInt64"
        }};

        template<int N>
        bool checkIsValidType(const std::array<std::string, N>& valid_types, const std::string& specified_type) {
            bool is_valid = false;

            for(const auto& t : valid_types) {
                if (t == specified_type) {
                    is_valid = true;
                }
            }

            return is_valid;
        }

        bool checkType(const std::string& type) {
            bool isValid = false;
            switch (current_tag) {
                case TAG::VTKFile:
                    isValid = checkIsValidType<VTKFile_type_length>(VTKFileType, type);
                    break;
                case TAG::DataArray:
                    isValid = checkIsValidType<number_type_length>(NumberType, type);
                    break;
                case TAG::PDataArray:
                    isValid = checkIsValidType<number_type_length>(NumberType, type);
                    break;
                default:
                    isValid = true;
                    break;
            }

            if (!isValid) {
                std::string tagString = getCurrentTagString();

                std::string error_message = "[SIMPLE VTK ERROR] Invalid " + tagString + " type = " + type + " is passed to.";
                throw std::invalid_argument(error_message);
            }

            return isValid;
        }

        std::string getCurrentTagString() {
            switch (current_tag) {
                case TAG::VTKFile:
                    return "VTKFile";
                default:
                    return "";
            }
        }

        std::string convertTagString(const std::string& tag) {
            if (tag == "StructuredTopology" || tag == "UnstructuredTopology") {
                return "Topology";
            } else {
                return tag;
            }
        }

        std::string getConventionalFileExtensionFromCurrentVTKType() const {
            if (current_vtk_type == "ImageData") {
                return ".vti";
            } else if (current_vtk_type == "RectlinearGrid") {
                return ".vtr";
            } else if (current_vtk_type == "StructuredGrid") {
                return ".vts";
            } else if (current_vtk_type == "PolyData") {
                return ".vtp";
            } else if (current_vtk_type == "UnstructuredGrid") {
                return ".vtu";
            } else if (current_vtk_type == "vtkHierarchicalBoxDataSet") {
                return ".vthb";
            } else if (current_vtk_type == "PImageData") {
                return ".pvti";
            } else if (current_vtk_type == "PRectlinearGrid") {
                return ".pvtr";
            } else if (current_vtk_type == "PStructuredGrid") {
                return ".pvts";
            } else if (current_vtk_type == "PPolyData") {
                return ".pvtp";
            } else if (current_vtk_type == "PUnstructuredGrid") {
                return ".pvtu";
            } else if (current_vtk_type == "PvtkHierarchicalBoxDataSet") {
                return ".pvthb";
            } else {
                return ".unknown";
            }
        }

        // Adding Valid Attributes
        void addType(const std::string& type) {
            if (type == "") return;

            if (checkType(type)) {
                buffer += " type=\"" + type + "\"";

                if (current_tag == TAG::VTKFile) {
                    current_vtk_type = type;
                }
            }
        }

        // for convinience
        void convertFromVariadicArgsToStringInternal(const std::string& buffer) {}

        template<typename First, typename... Rests>
        void convertFromVariadicArgsToStringInternal(std::string& buffer, First&& first, Rests&&... rests) {
            std::stringstream ss;
            ss << first;
            buffer = ss.str() + buffer;

            constexpr std::size_t parameter_pack_size = sizeof...(Rests);
            if (parameter_pack_size > 0) {
                buffer = " " + buffer;
                convertFromVariadicArgsToStringInternal(buffer, std::forward<Rests>(rests)...);
            }
        }

        template<typename... Args>
        std::string convertFromVariadicArgsToString(Args&&... args) {
            std::string buffer = "";
            convertFromVariadicArgsToStringInternal(buffer, std::forward<Args>(args)...);
            return buffer;
        }

        void addItemInternal(std::stringstream& ss) {}

        template <typename First, typename... Rests>
        void addItemInternal(std::stringstream &ss, First &&first, Rests &&... rests) {
            ss << first;

            constexpr std::size_t parameter_pack_size = sizeof...(Rests);
            if (parameter_pack_size > 0) {
                ss << " ";
                addItemInternal(ss, std::forward<Rests>(rests)...);
            }
        }

    public:
        SimpleVTK() {
            initialize();
        }

        void setIndentSpaceSize(const int size = 4) {
            if (size == 0) {
                indent = '\t';
                return;
            }

            indent.clear();
            for (int i = 0; i < size; ++i) {
                indent += ' ';
            }
        }

        void setNewLineCodeLF() {
            constexpr char LF = '\n';
            newLine = LF;
        }

        void setNewLineCodeCR() {
            constexpr char CR = '\r';
            newLine = CR;
        }

        void setNewLineCodeCRLF() {
            constexpr const char* CRLF = "\r\n";
            newLine = CRLF;
        }

        // IO functions
        void generate(const std::string file_name) {
            if(!endEdit) endVTK();

            std::ofstream ofs(file_name + getConventionalFileExtensionFromCurrentVTKType(), std::ios::out);
            ofs << content;
        }

        std::string getRawString() const {
            return content;
        }

        void beginVTK(const std::string& type) {
            beginElement("VTKFile");
            addType(type);
        }

        void endVTK() {
            endElement("VTKFile");
            endEdit = true;
        }

        template<typename T>
        void addArray(const T* values_ptr, const int N) {
            beginInnerElement();

            std::stringstream ss;
            for(size_t i = 1; i <= N; ++i) {
                ss << values_ptr[i - 1];

                if (i < N) {
                    ss << " ";

                    if (i % innerElementPerLine == 0) {
                        buffer += ss.str() + newLine;
                        insertIndent();
                        ss.str("");
                        ss.clear(std::stringstream::goodbit);
                    }
                }
            }

            if (ss.str() != "") {
                buffer += ss.str() + newLine;
            }

            endInnerElement();
        }

        template<typename T>
        void add2DArray(T** values_ptr, const int nx, const int ny) {
            beginInnerElement();

            std::stringstream ss;

            size_t itr = 1;
            size_t size = nx * ny;

            for(size_t j = 1; j <= ny; ++j) {
                for(size_t i = 1; i <= nx; ++i) {
                    ss << values_ptr[i - 1][j - 1];

                    if (itr < size) {
                        ss << " ";

                        if (itr % innerElementPerLine == 0) {
                            buffer += ss.str() + newLine;
                            insertIndent();
                            ss.str("");
                            ss.clear(std::stringstream::goodbit);
                        }
                    }

                    ++itr;
                }
            }

            if (ss.str() != "") {
                buffer += ss.str() + newLine;
            }

            endInnerElement();
        }

        template<typename T, size_t N>
        void addArray(const std::array<T, N>& values) {
            beginInnerElement();

            std::stringstream ss;
            for(size_t i = 1; i <= N; ++i) {
                ss << values[i - 1];

                if (i < N) {
                    ss << " ";

                    if (i % innerElementPerLine == 0) {
                        buffer += ss.str() + newLine;
                        insertIndent();
                        ss.str("");
                        ss.clear(std::stringstream::goodbit);
                    }
                }
            }

            if (ss.str() != "") {
                buffer += ss.str() + newLine;
            }

            endInnerElement();
        }

        template<typename T>
        void addVector(const std::vector<T>& values) {
            beginInnerElement();

            std::stringstream ss;
            auto size = values.size();
            for(size_t i = 1; i <= size; ++i) {
                ss << values[i - 1];

                if (i < size) {
                    ss << " ";

                    if (i % innerElementPerLine == 0) {
                        buffer += ss.str() + newLine;
                        insertIndent();
                        ss.str("");
                        ss.clear(std::stringstream::goodbit);
                    }
                }
            }

            if (ss.str() != "") {
                buffer += ss.str() + newLine;
            }

            endInnerElement();
        }

#ifdef USE_BOOST
        // based on c_index_order (row-major)
        template<typename T, size_t N>
        void addMultiArray(const boost::multi_array<T, N>& values, const bool isFortranStorageOrder = false) {
            const int size = values.num_elements();

            if (isFortranStorageOrder) {
                addArray(values.data(), size);
            } else {
                using array_type = boost::multi_array<T, N>;
                boost::array<typename array_type::index, N> index;

                // initialize index
                for(int i = 0; i < N; ++i) {
                    index[i] = 0;
                }
                auto shape = values.shape();

                // capture index and shape
                std::function<void(int)> proceedIndex = [&index, &shape, &proceedIndex](const int axis){
                    if (axis < N) {
                        // increments from the most left index
                        index[axis] += 1;
                        if (index[axis] == shape[axis]) {
                            index[axis] = 0;
                            proceedIndex(axis + 1);
                        }
                    }
                };

                beginInnerElement();
                std::stringstream ss;
                for (size_t i = 1; i <= size; ++i) {
                    ss << values(index);
                    proceedIndex(0);

                    if (i < size) {
                        ss << " ";

                        if (i % innerElementPerLine == 0) {
                            buffer += ss.str() + newLine;
                            insertIndent();
                            ss.str("");
                            ss.clear(std::stringstream::goodbit);
                        }
                    }
                }

                if (ss.str() != "") {
                    buffer += ss.str() + newLine;
                }

                endInnerElement();
            }
        }
#endif

        template<typename... Args>
        void addItem(Args&&... args) {
            beginInnerElement();

            std::stringstream ss;
            addItemInternal(ss, std::forward<Args>(args)...);
            buffer += ss.str() + newLine;

            endInnerElement();
        }

        // --- Attirbute Setting Functions ---
        void setName(const std::string& name) {
            if (name != "") {
                buffer += " Name=\"" + name + "\"";
            }
        }

        void setVersion(const std::string& _version) {
            buffer += " version=\"" + _version + "\"";
        }

        void setFormat(const std::string& type = "XML") {
            if (checkIsValidType<format_type_length>(FormatType, type)) {
                buffer += " Format=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE VTK ERROR] Invalid Format type = " + type + " is passed to setFormat().";
                throw std::invalid_argument(error_message);
            }
        }

        void setType(const std::string& type = "Float32") {
            if (checkIsValidType<number_type_length>(NumberType, type)) {
                buffer += " type=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE VTK ERROR] Invalid Number type = " + type + " is passed to setType().";
                throw std::invalid_argument(error_message);
            }
        }

        //*/

        template<typename... Args>
        void setDimensions(Args&&... args) {
            std::string dimString = convertFromVariadicArgsToString(std::forward<Args>(args)...);
            buffer += " Dimensions=\"" + dimString + "\"";
        }

        template<typename... Args>
        void setNumberOfElements(Args&&... args) {
            std::string dimString = convertFromVariadicArgsToString(std::forward<Args>(args)...);
            buffer += " NumberOfElements=\"" + dimString + "\"";
        }

        /*/
        // helper functoins
        void begin2DStructuredGrid(const std::string& gridName, const std::string& topologyType, const int nx, const int ny) {
            beginGrid(gridName);

            beginStructuredTopology("", topologyType);
            setNumberOfElements(nx, ny);
            endStructuredTopology();
        }
        
        void end2DStructuredGrid() {
            endGrid();
        }

        template<typename T>
        void add2DGeometryOrigin(const std::string& geomName, const T origin_x, const T origin_y, const T dx, const T dy) {
            beginGeometory(geomName, "ORIGIN_DXDY");

            // Origin
            beginDataItem();
            setDimensions(2);
            setFormat("XML");
            addItem(origin_y, origin_x);
            endDataItem();

            // Strands
            beginDataItem();
            setDimensions(2);
            setFormat("XML");
            addItem(dy, dx);
            endDataItem();

            endGeometory();
        }

        void begin3DStructuredGrid(const std::string& gridName, const std::string& topologyType, const int nx, const int ny, const int nz) {
            beginGrid(gridName);

            beginStructuredTopology("", topologyType);
            setNumberOfElements(nx, ny, nz);
            endStructuredTopology();
        }
        
        void end3DStructuredGrid() {
            endGrid();
        }

        template<typename T>
        void add3DGeometryOrigin(const std::string& geomName, const T origin_x, const T origin_y, const T origin_z, const T dx, const T dy, const T dz) {
            beginGeometory(geomName, "ORIGIN_DXDYDZ");

            // Origin
            beginDataItem();
            setDimensions(3);
            setFormat("XML");
            addItem(origin_z, origin_y, origin_x);
            endDataItem();

            // Strands
            beginDataItem();
            setDimensions(3);
            setFormat("XML");
            addItem(dz, dy, dx);
            endDataItem();

            endGeometory();
        }
        //*/
};

#endif