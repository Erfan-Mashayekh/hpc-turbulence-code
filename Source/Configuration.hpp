#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include "Parameters.hpp"
#include "Definitions.hpp"

#include <string>

namespace NSEOF {

class Configuration{
private:
    int dim_;
    std::string filename_;

public:
    Configuration();
    Configuration(const std::string& filename);
    ~Configuration() = default;

    void setFileName(const std::string& filename);
    void loadParameters(Parameters& parameters, const MPI_Comm& communicator = PETSC_COMM_WORLD);
};

} // namespace NSEOF

#endif // __CONFIGURATION_HPP__
