// RUN: %clang_cc1 -std=c++17 -load %llvmshlibdir/StructInfoPlugin%pluginext -plugin struct-info %s 2>&1 | FileCheck %s

// CHECK: CatBreed
// CHECK: breedName (const char *|public)

// CHECK: CatRecord
// CHECK: isNeutered (bool|public)
// CHECK: firstLetter (char|public)
// CHECK: ageMonths (signed char|public)
// CHECK: pawCount (unsigned char|public)
// CHECK: weightKg (short|public)
// CHECK: heightCm (unsigned short|public)
// CHECK: microchipId (int|public)
// CHECK: registrationNum (unsigned int|public)
// CHECK: uniqueId (long|public)
// CHECK: tagNumber (unsigned long|public)
// CHECK: dnaSequence (long long|public)
// CHECK: genomeBits (unsigned long long|public)
// CHECK: furDensity (float|public)
// CHECK: bodyTemperature (double|public)
// CHECK: preciseWeight (long double|public)
// CHECK: name (const char *|public)
// CHECK: scoreHistory (int *|public)
// CHECK: clawLengths (int[4]|public)
// CHECK: color (CatColor|public)
// CHECK: breed (CatBreed|public)

enum CatColor { Black, White, Orange, Tabby };

struct CatBreed {
  const char *breedName;
};

struct CatRecord {
  bool          isNeutered;
  char          firstLetter;
  signed char   ageMonths;
  unsigned char pawCount;
  short         weightKg;
  unsigned short heightCm;
  int           microchipId;
  unsigned int  registrationNum;
  long          uniqueId;
  unsigned long tagNumber;
  long long     dnaSequence;
  unsigned long long genomeBits;
  float         furDensity;
  double        bodyTemperature;
  long double   preciseWeight;
  const char   *name;
  int          *scoreHistory;
  int           clawLengths[4];
  CatColor      color;
  CatBreed      breed;
};
