# Constantes
INPUT_MINIMO = 0
INPUT_MAXIMO = 100
OUTPUT_MINIMO = 0
OUTPUT_MAXIMO = 389

"""
    Programa para obtener array que se utilizará como Look Up Table (LUT).
    
    Input: Duty Cycle deseado
    Output: equivalente en regla de tres (INPUT_MAXIMO es a OUTPUT_MAXIMO como ... es a ...)
"""

# Funciones 
def mapeo(valor, in_min, in_max, out_min, out_max):
    return (int)( (valor-in_min)*(out_max - out_min)/(in_max - in_min) + out_min)

# Programa
print("const uint8_t RGB_DUTY_CYCLE_LUT [] = {", end="")

for i in range(INPUT_MAXIMO + 1):
    print_str = f"{mapeo(i, INPUT_MINIMO, INPUT_MAXIMO, OUTPUT_MINIMO, OUTPUT_MAXIMO)}"
    if(i != INPUT_MAXIMO):
        print_str += ", "
    print(print_str, end="")
    
print("};")