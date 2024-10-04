#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/filesystem.hpp>

//# sudo apt-get install libpng-dev

namespace gil = boost::gil;

int main() {
  std::string inputfile = "input.png";
  std::string outputfile = "output.png";

  if (!boost::filesystem::exists( inputfile )) {
    std::cerr << "파일이 존재하지 않습니다: " << inputfile << std::endl;
    return 1;
  }

  gil::rgb8_image_t img;
  gil::read_image(inputfile, img, gil::png_tag());

  gil::gray8_image_t gray_img(img.dimensions());
  gil::copy_and_convert_pixels(gil::const_view(img), gil::view(gray_img));

  gil::write_view(outputfile, gil::const_view(gray_img), gil::png_tag());
  return 0;
}
