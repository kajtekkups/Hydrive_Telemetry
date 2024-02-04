#ifndef ADC_HPP
#define ADC_HPP


/* rozdzielczosc modulu pomiaru pradu wynosi 100A natomiast rozdzielczość ADC 2048, 
* jednak nasze napiecie referencyjne to 5V a ADC mierzy 0-6,144V,
*  dlatego realna rozdzielczosc to 1666,67, co mozna policzyc z prostej proporcji:
*             6,144 = 2048
*                5  =   x
*/
#define ROZDZIELCZOSC_PRZETWORNIKA_ADC 1666.67 
#define NAPIECIE_REFERENCYJNE 5
#define ACS758_SENSITIVITY_DEFAULT 0.04  // wrazliwosc czujnika wyrazona w V/A
#define BLAD_POMIARU 0.65


float CalculateAmp(float Measure);

/* przelicza wartosc otrzymana z przetwornika 
 * na zmierzone napiecie, dla przetwornika Grove 5/43 V
*/
float CalculateVolt_for_meter(float Measure);
 
#endif