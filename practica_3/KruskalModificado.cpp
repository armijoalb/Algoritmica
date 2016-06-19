#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>


using namespace std;
using namespace std::chrono;

class Clock{
private:
	// Holds the start time
	high_resolution_clock::time_point start_time;

public:
	Clock(){
		start();
	}

	void start(){
		/*
			Set the 'start' time to the current time.
		*/
		start_time = high_resolution_clock::now();
	}

	double now(){
		/*
			It returns a double with the time between start and the moment when you call it.
		*/
		high_resolution_clock::time_point end = high_resolution_clock::now();
		duration<double> time = duration_cast<duration<double>>(end - start_time);

		return time.count();
	}
};



// -------------------------------------------------------------------------------------------

typedef std::pair<int, int> ciudades;

class comp_pares{
  public:
  bool operator() (const std::pair<double ,ciudades> &lhs,
                    const std::pair<double ,ciudades> &rhs ){

  return lhs.first > rhs.first;
  }
};

// Función para comprobar si añadir una arista formaría un ciclo.
bool crea_ciclos( vector<ciudades> &s, ciudades &arista ){
  bool crea_ciclo = false;
  bool encontrado_first = false,
       encontrado_second = false;

  int pos_first, pos_second;

  bool first_izq, second_izq;
  vector<ciudades> copia = s;
  ciudades a;

  // TODO: comprobar que no hay camino posible desde arista.first hasta arista.second
  // pasando por otras ciudades.

  for( int i = 0 ; i < s.size() and !(encontrado_first and encontrado_second); i++ ){
    if( arista.first == s[i].first or arista.first == s[i].second ){
      encontrado_first = true;
      pos_first = i;

      if( arista.first == s[i].first )
        first_izq = true;
      else
        first_izq = false;

    }
    if( arista.second == s[i].second or arista.second == s[i].first ){
      encontrado_second = true;
      pos_second = i;

      if( arista.second == s[i].first )
        second_izq = true;
      else
        second_izq = false;
    }
  }

  //cout << pos_first << " " << pos_second << endl;
  // Si hemos encontrado otros pares de nodos desde los que se podría llegar,
  // seguimos, sino, finalizamos.
  while( encontrado_first and encontrado_second and !crea_ciclo){
    if( first_izq and second_izq ){
      //cout << "Regla 1" << endl;
      if( copia[pos_first].second == copia[pos_second].second or pos_first == pos_second)
        crea_ciclo = true;
      else{
        a.first = copia[pos_first].second;
        a.second = copia[pos_second].second;
        if( pos_first > pos_second ){
          copia.erase( copia.begin() + pos_first );
          copia.erase( copia.begin() + pos_second );
        }
        else{
          copia.erase( copia.begin() + pos_second );
          copia.erase( copia.begin() + pos_first );
        }
      }
    }
    else if( first_izq and !second_izq ){
      //cout << "Regla 2" << endl;
      if( copia[pos_first].second == copia[pos_second].first or pos_first == pos_second)
        crea_ciclo = true;
      else{
        a.first = copia[pos_first].second;
        a.second = copia[pos_second].first;
        if( pos_first > pos_second ){
          copia.erase( copia.begin() + pos_first );
          copia.erase( copia.begin() + pos_second );
        }
        else{
          copia.erase( copia.begin() + pos_second );
          copia.erase( copia.begin() + pos_first );
        }
      }
    }
    else if( !first_izq and second_izq ){
      //cout << "Regla 3" << endl;
      if( copia[pos_first].first == copia[pos_second].second or pos_first == pos_second)
        crea_ciclo = true;
      else{
        a.first = copia[pos_first].first;
        a.second = copia[pos_second].second;
        if( pos_first > pos_second ){
          copia.erase( copia.begin() + pos_first );
          copia.erase( copia.begin() + pos_second );
        }
        else{
          copia.erase( copia.begin() + pos_second );
          copia.erase( copia.begin() + pos_first );
        }
      }
    }
    else{
      //cout << "Regla 4" << endl;
      if( copia[pos_first].first == copia[pos_second].first or pos_first == pos_second)
        crea_ciclo = true;
      else{
        a.first = copia[pos_first].first;
        a.second = copia[pos_second].first;

        if( pos_first > pos_second ){
          copia.erase( copia.begin() + pos_first );
          copia.erase( copia.begin() + pos_second );
        }
        else{
          copia.erase( copia.begin() + pos_second );
          copia.erase( copia.begin() + pos_first );
        }
      }
    }
    // Volvemos a buscar.
    encontrado_first = false;
    encontrado_second = false;

    for( int i = 0 ; i < copia.size() and !(encontrado_first and encontrado_second); i++ ){
      if( a.first == copia[i].first or a.first == copia[i].second ){
        encontrado_first = true;
        pos_first = i;

        if( a.first == copia[i].first )
          first_izq = true;
        else
          first_izq = false;

      }
      if( a.second == copia[i].second or a.second == copia[i].first ){
        encontrado_second = true;
        pos_second = i;

        if( a.second == copia[i].first )
          second_izq = true;
        else
          second_izq = false;
      }
    }

  }

  return crea_ciclo;
}

vector<ciudades> ViajanteComercioKruskal(double **m, int n){
  vector<ciudades> solucion;

  int grado[n] = {0};
  // Ponerla para que ordene según orden decreciente.
  priority_queue< std::pair<double ,ciudades>, std::vector<std::pair<double, ciudades> >, comp_pares > seleccionados;
  ciudades c;
  int pos_first = -1, pos_second = -1;
  // Introducimos los datos en la cola.
  for(int i = 0; i < n; i++)
    for( int j = i+1; j < n ; j++){
      c.first = i;
      c.second = j;
      seleccionados.push( make_pair(m[i][j], c) );
    }

  while( !seleccionados.empty() ){
    ciudades arista = seleccionados.top().second;

    //cout << "------------------------------------------------------------" << endl;
    //cout << "( " << arista.first << "," << arista.second << ")" ;
    //cout << " ---> Grado: ( " << grado[arista.first] << "," << grado[arista.second] << ")" << endl;

    if( grado[arista.first] < 2 and grado[arista.second] < 2 ){

          if( !crea_ciclos( solucion, arista ) ){
            //cout << "no crea ciclos. Lo incluimos en la solución" << endl;
            solucion.push_back( seleccionados.top().second );
            // Aumentamos el grado de cada uno de los nodos.
            grado[seleccionados.top().second.first] ++;
            grado[seleccionados.top().second.second] ++;
          }
          else{
            //cout << "Crea ciclos" << endl;
          }
        }

    seleccionados.pop();
  }

  // Buscamos los últimos dos ciudades con grado 1, y las introducimos dentro de la solución.
  for(int i = 0 ; i < n ; i++){
    if( grado[i] == 1 ){
      if(pos_first == -1)
        pos_first = i;
      else
        pos_second = i;
    }
  }

  solucion.push_back( make_pair(pos_first, pos_second) );

  return solucion;
}

vector<int> transformarAVectorCiudades( vector<pair<int, int> > &aristas ){
  vector<int> salida;
  bool encontrado = false;
  int a_buscar;

  vector<pair<int, int> > copia = aristas;


  // Metemos el primer elemento de las aristas.
  salida.push_back( copia[0].first );
  //cout << "introducido: " << copia[0].first << endl;
  salida.push_back( copia[0].second );
  //cout << "introducido: " << copia[0].second << endl;
  a_buscar = copia[0].second;


  copia.erase( copia.begin() );

  while( salida.size() < aristas.size() ){
    //cout << "--------------------------------------------------------------";
    //cout << "TAM :" << salida.size() << endl;
    //cout << "Ciudad a buscar: " << a_buscar << endl;

    encontrado = false;

    for( auto it = copia.begin() ; it != copia.end() && !encontrado ; it ++){

      //cout << (*it).first << ' ' << (*it).second << endl;

      if( a_buscar == (*it).first ){
        a_buscar = (*it).second;
        salida.push_back( a_buscar );
        encontrado = true;
        //cout << "introducido: " << (*it).second << endl;

        copia.erase( it );
      }
      else if( a_buscar == (*it).second ){
        a_buscar = (*it).first ;
        salida.push_back( a_buscar );
        encontrado = true;
        //cout << "introducido: " << (*it).first << endl;

        copia.erase( it );
      }

    }

  }

  return salida;
}

int calcularPeso( vector<int> &ciudades, double **m_pesos){
  int peso = 0;

  for( int i = 0; i < ciudades.size()-1 ; i++){
    peso += m_pesos[ciudades[i]][ciudades[i+1]];
  }

  peso += m_pesos[ciudades[0]][ciudades[ciudades.size()-1]];

  return peso;
}

// -------------------------------------------------------------------------------------------

double Distancia( pair<double,double> A, pair<double,double> B ){

    double distancia_d;
    int distancia_i;

    distancia_d = sqrt( pow( B.first - A.first, 2 )  + pow( B.second - A.second, 2 ));

    //distancia_i = round( distancia_d );

    return distancia_d;
}

/*
double DistanciaTotal( vector< pair<double,double> > & V ){

    double distancia = 0;

    for( int i = 0; i < V.size - 1; i++){

        distancia += Distancia( V[i], V[i+1] );

    }

    distancia += Distancia( V[ V.size()], v[0]);


}
*/
double DistanciaTotal( vector< int > & V, double **M ){

    double distancia = 0;

    for( int i = 0; i < V.size() -1 ; i++)
        distancia +=  M[ V[i] ][ V[i+1] ]  ;

    distancia += M[ V[ V.size()-1 ] ][ V[0] ]  ;

}



void GeneraMatrizDistancias( vector< pair<double,double> > & V, double **M ){


    for( int i = 0; i < V.size(); i++)
        for( int j = i; j < V.size(); j++ )
            M[j][i] = M[i][j] = Distancia( V[i], V[j]) ;



}

void LecturaFichero( vector<pair<double, double> > & V, char * fic){

   ifstream ifile;

   pair<double, double> coord;

   ifile.open(fic);
   ifile.ignore(11, 32); // Ignorar "Dimensión: "

   // Variables a leer
   int n_cities = 0;
   double _x, _y;
   int n;

   ifile >> n_cities;

   for(int i = 0; i < n_cities && !ifile.eof(); i++){
      ifile >> n;
      ifile >> _x;
      ifile >> _y;
      coord = make_pair(_x, _y);
      V.push_back(coord);
   }


}
