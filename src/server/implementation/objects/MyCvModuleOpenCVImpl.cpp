/* Autogenerated with kurento-module-creator */

#include "MyCvModuleOpenCVImpl.hpp"
#include <KurentoException.hpp>

#define GST_CAT_DEFAULT kurento_my_cv_module_impl
GST_DEBUG_CATEGORY_STATIC(GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoMyCvModuleImpl"


namespace kurento
{
namespace module
{
namespace mycvmodule
{

MyCvModuleOpenCVImpl::MyCvModuleOpenCVImpl ()
{
  GST_DEBUG_CATEGORY_INIT(GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0, GST_DEFAULT_NAME);

  this->snapInterval = 1000;
  this->lastQueueTimeStamp = 0;
  this->framesCounter = 0;
  this->storagePath = "/home/ak/frames";
}

/*
 * This function will be called with each new frame. mat variable
 * contains the current frame. You should insert your image processing code
 * here. Any changes in mat, will be sent through the Media Pipeline.
 */
void MyCvModuleOpenCVImpl::process (cv::Mat &mat)
{
  if ((this->getCurrentTimestampLong() - this->lastQueueTimeStamp) >= this->snapInterval) {
    std::vector<int> params;
    this->lastQueueTimeStamp = this->getCurrentTimestampLong();

    /* Set jpeg params */
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(FG_JPEG_QUALITY);
    std::string filename = std::to_string((long) this->framesCounter) + "_" + std::to_string(this->getCurrentTimestampLong()) + ".jpeg";

    if (this->storagePathSubdir.empty()) {
      this->storagePathSubdir = this->storagePath + "/frames_" + this->getCurrentTimestampString();
      boost::filesystem::path dir(this->storagePathSubdir.c_str());
      GST_INFO("going to create a directory in %s", this->storagePathSubdir.c_str());
      if (!boost::filesystem::create_directories(dir)) {
        GST_ERROR("create_directories() failed for: %s", this->storagePathSubdir.c_str());
      }
    }

    std::string fullpath = this->storagePathSubdir + "/" + filename;

    try {
      cv::imwrite(fullpath.c_str(), mat, params);
    }
    catch (...) {
      GST_ERROR("::queueHandler() imgwrite() failed.");
    }

    ++this->framesCounter;
  }

  // FIXME: Implement this
  //throw KurentoException (NOT_IMPLEMENTED, "MyCvModuleOpenCVImpl::process: Not implemented");
}

std::string MyCvModuleOpenCVImpl::getCurrentTimestampString() {
  struct timeval tp;
  long int ms;
  std::stringstream sstr_ts;

  gettimeofday(&tp, NULL);
  ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  sstr_ts << ms;
  return sstr_ts.str();
}

long MyCvModuleOpenCVImpl::getCurrentTimestampLong() {
  struct timeval tp;

  gettimeofday(&tp, NULL);
  return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}


} /* mycvmodule */
} /* module */
} /* kurento */
