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
        /*/
        static constexpr int dataItemTypeLength = 6;
        static constexpr int gridTypeLength = 4;
        static constexpr int structuredTopologyTypeLength = 6;
        static constexpr int unstructuredTopologyTypeLength = 17;
        static constexpr int geometryTypeLength = 6;
        static constexpr int attributeTypeLength = 5;
        static constexpr int attributeCenterLength = 5;
        static constexpr int setTypeLength = 4;
        static constexpr int timeTypeLength = 4;
        static constexpr int formatTypeLength = 3;
        static constexpr int numberTypeLength = 5;
        static constexpr int precisionTypeLength = 4;

        std::array<std::string, dataItemTypeLength> DataItemType {{"Uniform", "Collection", "Tree", "HyperSlab", "Coordinates", "Function"}};
        std::array<std::string, gridTypeLength> GridType {{"Uniform", "Collection", "Tree", "Subset"}};
        std::array<std::string, structuredTopologyTypeLength> StructuredTopologyType {{"2DSMesh", "2DRectMesh", "2DCoRectMesh", "3DSMesh", "3DRectMesh", "3DCoRectMesh"}};
        std::array<std::string, unstructuredTopologyTypeLength> UnstructuredTopologyType {{"Polyvertex", "Polyline", "Polygon", "Triangle", "Quadrilateral", "Tetrahedron", "Pyramid", "Wedge", "Hexahedron", "Edge_3", "Tri_6", "Quad_8", "Tet_10", "Pyramid_13", "Wedge_15", "Hex_20", "Mixed"}};
        std::array<std::string, geometryTypeLength> GeometryType {{"XYZ", "XY", "X_Y_Z", "VXVYVZ", "ORIGIN_DXDYDZ", "ORIGIN_DXDY"}};
        std::array<std::string, attributeTypeLength> AttributeType {{"Scalar", "Vector", "Tensor", "Tensor6", "Matrix"}};
        std::array<std::string, attributeCenterLength> AttributeCenter {{"Node", "Edge", "Face", "Cell", "Grid"}};
        std::array<std::string, setTypeLength> SetType {{"Node", "Edge", "Face", "Cell"}};
        std::array<std::string, timeTypeLength> TimeType {{"Single", "HyperSlab", "List", "Range"}};
        std::array<std::string, formatTypeLength> FormatType {{"XML", "HDF", "Binary"}};
        std::array<std::string, numberTypeLength> NumberType {{"Float", "Int", "UInt", "Char", "UChar"}};
        std::array<std::string, precisionTypeLength> PrecisionType {{"1", "2", "4", "8"}};
        //*/

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
                    // isValid = checkIsValidType<gridTypeLength>(GridType, type);
                    isValid = true;
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
                /*/
                case TAG::DataItem:
                    return "DataItem";
                case TAG::StructuredTopology:
                    return "Topology";
                case TAG::UnstructuredTopology:
                    return "Topology";
                case TAG::Geometry:
                    return "Geometry";
                case TAG::Attribute:
                    return "Attribute";
                case TAG::Set:
                    return "Set";
                case TAG::Time:
                    return "Time";
                case TAG::Information:
                    return "Information";
                case TAG::Domain:
                    return "Domain";
                case TAG::Inner:
                    return "Inner";
                case TAG::Xdmf:
                    return "Xdmf";
                /*/
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
                buffer += " " + "type=\"" + type + "\"";

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

        /*/
        void setFormat(const std::string& type = "XML") {
            if (checkIsValidType<formatTypeLength>(FormatType, type)) {
                buffer += " Format=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Format type = " + type + " is passed to setFormat().";
                throw std::invalid_argument(error_message);
            }
        }

        void setPrecision(const std::string& type = "4") {
            if (checkIsValidType<precisionTypeLength>(PrecisionType, type)) {
                buffer += " Precision=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Precision type = " + type + " is passed to setPrecision().";
                throw std::invalid_argument(error_message);
            }
        }

        void setNumberType(const std::string& type = "Float") {
            if (checkIsValidType<numberTypeLength>(NumberType, type)) {
                buffer += " NumberType=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Number type = " + type + " is passed to setNumberType().";
                throw std::invalid_argument(error_message);
            }
        }

        void setCenter(const std::string& type = "Node") {
            if (current_tag != TAG::Attribute) {
                std::cerr << "[SIMPLE XDMF ERROR] setCenter() cannot be called when current Tag is not Attribute." << std::endl;
                return;
            }

            if (checkIsValidType<attributeCenterLength>(AttributeCenter, type)) {
                buffer += " Center=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Center type = " + type + " is passed to setCenter().";
                throw std::invalid_argument(error_message);
            }
        }

        void setFunction(const std::string& func) {
            if (current_tag != TAG::DataItem) {
                std::cerr << "[SIMPLE XDMF ERROR] setFunction() cannot be called when current Tag is not DataItem." << std::endl;
                return;
            }
            buffer += " Function=\"" + func + "\"";
        }

        void setSection(const std::string& sect) {
            if (current_tag != TAG::Grid) {
                std::cerr << "[SIMPLE XDMF ERROR] setSection() cannot be called when current Tag is not Grid." << std::endl;
                return;
            }

            if (sect == "DataItem" || sect == "All") {
                buffer += " Section=\"" + sect + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Section type = " + sect + " is passed to setSection().";
                throw std::invalid_argument(error_message);
            }
        }

        void setValue(const std::string& value) {
            if (current_tag != TAG::Time && current_tag != TAG::Information) {
                std::cerr << "[SIMPLE XDMF ERROR] setValue() cannot be called when current Tag is not Time and Information." << std::endl;
                return;
            }
            buffer += " Value=\"" + value + "\"";
        }
        /*/

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