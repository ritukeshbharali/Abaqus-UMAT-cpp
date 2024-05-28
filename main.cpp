/** Purpose: Load a Abaqus material routine dynamically,
 *           perform some operations.
 *  
 *  Procedure:
 *  gfortran -shared -o libumat.so umat.f
 *  g++ main.cpp -o main
 *  ./main
 */

// Import some libraries
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>

// Define the Abaqus UMAT function signature
typedef void (*umatFunc)(
    double* STRESS,        // Stress
    double* STATEV,        // State variables
    double* DDSDDE,        // Material tangent
    double* SSE,           // Specific elastic energy
    double* SPD,           // Specific plastic dissipation
    double* SCD,           // Specific creep dissipation
    double* RPL,           // Volumetric heat generation per unit time 
    double* DDSDDT,        // Tangent (stress w.r.t temperature)
    double* DRPLDE,        // Tangent (RPL w.r.t. strain)
    double* DRPLDT,        // Tangent (RPL w.r.t. temperature)
    double* STRAN,         // Strain
    double* DSTRAN,        // Strain increment
    double* TIME,          // Time
    double* DTIME,         // Time increment
    double* TEMP,          // Temperature
    double* DTEMP,         // Temperature increment
    double* PREDEF,        // Pre-defined field variables
    double* DPRED,         // Pre-defined field variables increment
    char*   CMNAME,        // Material name
    int*    NDI,           // # of direct stress components
    int*    NSHR,          // # of shear stress components
    int*    NTENS,         // Size of stress/strain array
    int*    NSTATV,        // # of state variables
    double* PROPS,         // Material properties
    int*    NPROPS,        // # of material properties
    double* COORDS,        // Coordinates of this point
    double* DROT,          // (3,3) rotation increment matrix
    double* PNEWDT,        // Ratio of new to old step-size
    double* CELENT,        // Characteristic element length
    double* DFGRD0,        // (3,3) Deformation grad at start of increment
    double* DFGRD1,        // (3,3) Deformation grad at end of increment
    int*    NOEL,          // Element number
    int*    NPT,           // Integration point number
    int*    LAYER,         // Layer number
    int*    KSPT,          // Section point number within the current layer
    int*    KSTEP,         // Step number
    int*    KINC           // Increment number
    );

// Define the shared library as a macro
#define UMAT_SHARED_LIB "libumat.so"

// main function
int main(int argc, char const *argv[])
{
  // Attempt to load the shared library 'libumat.so'
  // RLTD_NOW resolves all symbols upon loading.
  void* handle = dlopen(UMAT_SHARED_LIB, RTLD_NOW);

  // Check if 'handle' is a valid pointer
  if ( handle == nullptr )
  {
    std::cerr << "Error loading shared library: "
              << dlerror()
              << std::endl;
  }

  // Function pointer to UMAT
  umatFunc umat = reinterpret_cast<umatFunc>
                  ( dlsym( handle, "umat_" ) );

  // Check function pointer validity
  if ( umat == nullptr )
  {
    std::cerr << "Error locating UMAT function: "
              << dlerror()
              << std::endl;
  }

  // Initialize and define the UMAT variables
  // Garbage values are those that are defined
  // but not initialized. In the example code,
  // these values are not required. However, if
  // we want to develop something generic, they
  // need to be setup with correct dimensions
  // and values.

  double STRESS[6] = {0.0};     // Zero vector
  double STATEV;                // Garbage
  double DDSDDE[6][6] = {0.0};  // Zero matrix
  double SSE;                   // Garbage
  double SPD;                   // Garbage
  double SCD;                   // Garbage
  double RPL;                   // Garbage
  double DDSDDT;                // Garbage
  double DRPLDE;                // Garbage
  double DRPLDT;                // Garbage
  double STRAN[6] = {0.0};      // Zero vector
  double DSTRAN[6] = {0.0};     // Zero vector
  double TIME;                  // Garbage
  double DTIME;                 // Garbage
  double TEMP;                  // Garbage
  double DTEMP;                 // Garbage
  double PREDEF;                // Garbage
  double DPRED;                 // Garbage
  char   CMNAME;                // Garbage 
  int    NDI   = 3;             // 3 Direct stress
  int    NSHR  = 3;             // 3 Shear stress
  int    NTENS = 6;             // 6 (= NDI + NSHR)        
  int    NSTATV;                // Garbage int        
  double PROPS[2] = {100.,0.2}; // E, nu         
  int    NPROPS = 2;            // 2 properties        
  double COORDS;                // Garbage       
  double DROT;                  // Garbage       
  double PNEWDT;                // Garbage       
  double CELENT;                // Garbage       
  double DFGRD0;                // Garbage       
  double DFGRD1;                // Garbage       
  int    NOEL;                  // Garbage       
  int    NPT;                   // Garbage       
  int    LAYER;                 // Garbage       
  int    KSPT;                  // Garbage       
  int    KSTEP;                 // Garbage       
  int    KIN;                   // Garbage

  // Fill in some strain values
  STRAN[0] = 0.001;

  // Print imposed strain to terminal
  std::cout << "Imposed strain STRAN array:" << std::endl;
  for ( int i = 0; i<NTENS; i++ )
    std::cout << "STRAN[" << i << "] = "
              << STRAN[i] << std::endl;

  // Call the UMAT function
  umat(&STRESS[0], &STATEV, &DDSDDE[0][0], &SSE, &SPD, &SCD,
       &RPL, &DDSDDT, &DRPLDE, &DRPLDT, &STRAN[0], &DSTRAN[0], 
       &TIME, &DTIME, &TEMP, &DTEMP, &PREDEF, &DPRED, &CMNAME,
       &NDI, &NSHR, &NTENS, &NSTATV, &PROPS[0], &NPROPS, &COORDS,
       &DROT, &PNEWDT, &CELENT, &DFGRD0, &DFGRD1, &NOEL, &NPT,
       &LAYER, &KSPT, &KSTEP, &KIN);

  // To demonstrate that UMAT function works, we print out the
  // material tangent
  std::cout << "\nDDSDDE array after UMAT call:" << std::endl;
  for ( int i = 0; i<NTENS; i++ )
    for ( int j = 0; j<NTENS; j++ )
      std::cout << "DDSDDE[" << i << "," << j << "] = "
                << DDSDDE[i][j] << std::endl;

  // To demonstrate that UMAT function works, we print out the
  // stress
  std::cout << "\nSTRESS array after UMAT call:" << std::endl;
  for ( int i = 0; i<NTENS; i++ )
    std::cout << "STRESS[" << i << "] = "
              << STRESS[i] << std::endl;

	/* code */
	return 0;
}

