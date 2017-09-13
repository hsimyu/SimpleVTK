#include <test_simple_vtk.hpp>

namespace TEST_SIMPLE_VTK {

    TEST_F(Test1, check_header) {
        ASSERT_EQ(gen.getRawString(), header);
    }

    TEST_F(Test1, make_vtk_imagedata) {
        gen.beginVTK("ImageData");
        gen.endVTK();

        const std::string target = header + "<VTKFile type=\"ImageData\">\n</VTKFile>\n";

        std::cout << "----- current content -----" << std::endl;
        std::cout << gen.getRawString() << std::endl;
        std::cout << "----- target content -----" << std::endl;
        std::cout << target << std::endl;
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, make_vtk_imagedata_with_attributes) {
        gen.beginVTK("ImageData");
        gen.setByteOrder("LittleEndian");
        gen.setVersion("1.1");
        gen.setCompressor("vtkTestCompressor");
        gen.endVTK();

        const std::string target = header + "<VTKFile type=\"ImageData\" byte_order=\"LittleEndian\" version=\"1.1\" compressor=\"vtkTestCompressor\">\n</VTKFile>\n";

        std::cout << "----- current content -----" << std::endl;
        std::cout << gen.getRawString() << std::endl;
        std::cout << "----- target content -----" << std::endl;
        std::cout << target << std::endl;
        ASSERT_EQ(gen.getRawString(), target);
    }
}