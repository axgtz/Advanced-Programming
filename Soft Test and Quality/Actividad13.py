def union2D(elements, ref):
  # elements: List[List[int]]
  
  elements.sort(key = lambda x: x[0], reverse=True) 
  
  elements[0][0] = abs(elements[0][0] - ref[0])
  elements[0][1] = abs(elements[0][1] - ref[1])
  
  max_y = elements[0][1]
  
  union = elements[0][0] * elements[0][1]
  
  for index in range(1, len(elements)):
    elements[index][1] = abs(elements[index][1] - ref[1])
  
    if elements[index][1] > max_y:
      new_height = elements[index][1] - max_y
      max_y = elements[index][1]
      union += elements[index][0] * new_height
   
  return union

def openFile(file_name):
  e = []
  f = open(file_name, "r")

  lines = f.readlines()

  for line in lines:
    el = [float(i) for i in line.split(" ")]
    e.append(el)
  
  return e


# Error: La lectura del archivo no servirá correctamente y no voy a obtener los valores correctos
# Hipotesis: Al leer el archivo, el número se guarda como string en lugar de como float entonces no se pueden hacer operaciones con éste.

# Error: La referencia provocará que el resultado no sea correcto
# Hipótesis: No se tomó en cuenta la referencia para hacer las operaciones con las coordenadas

# Error: El algoritmo no funcione cuando los elementos no estén ordenados
# Hipótesis: No se ordenaron los elementos antes de ejecutar el algoritmo

if __name__ == "__main__":
  # e = [[1,4],[2,3],[3,2],[4,1]]
  e = openFile("set_2D.txt")
  print(e)
  print(union2D(e,[0,0]))