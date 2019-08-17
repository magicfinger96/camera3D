#ifndef libgeo_andrea
#define libgeo_andrea
#include <iostream>
#include <math.h>
#include <initializer_list>
#include "libmatrix.h"

using namespace std;
using namespace libmatrix;

namespace libgeometry {

	// Plan d'équation ax + by + cz + d = 0 avec le point (x,y,z)
	template<typename T>
	class Plane {
		private:
			Vector<T,4> constantes;

		public:

			// Constructeur
			Plane(){}

			// Constructeur prenant les constantes du plan
			Plane(Vector<T,4> constantes) : constantes(constantes){};

			Plane(T list[4]){
				constantes = Vector<T,4>(list);
			}

			// Retourne vrai si le plan contient des valeurs invalides
			bool is_null(){
				return constantes.is_null();
			}

			// Retourne le vecteur normal du plan
			Vector<T,3> getVector(){
				return Vector<T,3>({constantes[0],constantes[1],constantes[2]});
			}

			Plane<T> to_norm(){
				Vector<float,3> vec = getVector().to_unit();
				return Plane<T>({vec[0],vec[1],vec[2],constantes[3]});
			}

			// Retourne la constante d de l'équation du plan
			T getD(){
				return constantes[3];
			}

			// Retourne les constantes de l'équation du plan
			Vector<T,4> getConstantes() const {
				return constantes;
			}

			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const Plane<T> &plane){
				s << plane.constantes;
				return s;
			}
		
	};


	// Direction de dimension N et de type T
	template<typename T, size_t N>
	class Direction {
		private:
			Vector<T,N> coords; 
		public:
			// Constructeur
			Direction(){}			

			// Constructeur
			Direction(Vector<T,N> c) : coords(c) {}

			// Constructeur avec une liste en param
			Direction(T list[N]){
				coords = Vector<T,N>(list);
			}

			// Retourne vrai si la direction contient une valeur invalide
			bool is_null(){
				return coords.is_null();
			}

			// Retourne vrai si la direction est unitaire
			bool is_unit(){
				return coords.is_unit();
			}

			// Retourne une direction unitaire
			Direction<T,N> to_unit(){
				return Direction<T,N>(coords.to_unit());
			}

			// Retourne la norme de la direction
			T norm(){
				return coords.norm();
			}

			// Retourne les coordonnées de la direction
			Vector<T,N> getCoords() const {
				return coords;
			}

			// Multiplie la direction par une autre direction
			Direction<T,N> cross(const Direction<T,N> &direction2){
				return Direction<T,N>(coords.cross(direction2.coords));
			}

			// Retourne le ième composant de la direction
			T at(const int i){
				return coords.at(i);
			}

			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const Direction<T,N> &direction){
				s << direction.coords;
				return s;
			}

			// Adresse le ième composant de la direction et permet l'affectation.
			T &operator [] (const int i){
				return this->coords[i];
			}

			// Multiplie la direction par un scalaire
			Direction<T,N> operator* (const float scalaire){
				return Direction<T,N>(coords*scalaire);
			}

			// Produit scalaire entre la direction et un plan
			T operator* (const Plane<T> &plane){
				if(N != 4){
					throw runtime_error("La direction doit etre en 4D");
				}
				return plane.getConstantes() * coords;
			}
	};

	// Quaternion dont les composants sont de type T
	template<typename T>
	class Quaternion {
		private:
			Vector<T,4> elts;

		public:

			// Constructeur
			Quaternion(){}

			//Constructeur
			Quaternion(Vector<T,4> elts) : elts(elts) {};

			// Constructeur avec rotation et direction
			Quaternion(const float angle, Direction<T,3> axe){
				(*this) = Quaternion<T>(cos((angle*(M_PI / 180.f))/2.0f),(axe*(sin((angle*(M_PI / 180.f))/2.0f))).getCoords());
			}

			// Constructeur avec les 2 parties distinctes
			Quaternion(T re, Vector<T,3> im){
				T list[4];
				list[0]=re;
				for(int i=1;i<4;i++){
					list[i]=im[i-1];
				}
				elts = Vector<T,4>(list);
			};

			// Retourne le conjugué du quaternion
			Quaternion<T> conjugate(){
				Vector<T,4> vec({elts[0],-elts[1],-elts[2],-elts[3]});
				return Quaternion<T>(vec);
			}

			// Retourne la norme du quaternion
			T norm(){
				return elts.norm();
			}

			// Retourne la partie imaginaire du quaternion
			Vector<T,3> im(){
				Vector<T,3> res({elts[1],elts[2],elts[3]});
				return res;
			}

			// Retourne l'inverse du quaternion
			Quaternion<T> inverse(){
				return conjugate()*(1/(pow(norm(),2)));
			}

			// Retourne la partie réelle du quaternion
			T re(){
				return elts[0];
			}

			// Retourne une copie du quaternion normalisé
			Quaternion<T> to_norm(){
				return Quaternion<T>(elts.to_unit());
			}

			// Retourne l'angle de rotation du quaternion
			T getAngle() const {
				return (acos(elts.at(0))*180*2.f)/M_PI;
			}

			// Retourne l'axe de rotation du quaternion
			Direction<T,3> getAxis(){
				float coef = sin((getAngle()*(M_PI / 180.f))/2.f);
				if(coef == 0.0f){
					return Direction<float,3>(Vector<float,3>({elts.at(1),elts.at(2),elts.at(3)}));
				}
				return Direction<float,3>(Vector<float,3>({elts.at(1)/coef,elts.at(2)/coef,elts.at(3)/coef}));
			} 

			// Surchage de << pour affichage
			friend ostream &operator << (ostream &s, const Quaternion<T> &quaternion){
				s << quaternion.elts;
				return s;
			}
			
			// Multiplie le quaternion avec un scalaire
			Quaternion<T> operator * (const float scalaire){
				return Quaternion<T>(elts*scalaire);
			}

			// Multiplie le quaternion avec quaternion2
			Quaternion<T> operator * (Quaternion<T> quaternion2){
				
				T re = quaternion2.re()*elts[0] - im() * quaternion2.im();
				Vector<T,3> elem2 = elts[0]*quaternion2.im(); 
				Vector<T,3> elem3 = quaternion2.re()*im();
				Vector<T,3> elem4 = im().cross(quaternion2.im());
				return Quaternion<T>(re,elem2+elem3+elem4);
			}

			// Multiplie un scalaire avec le quaternion
			friend Quaternion<T>  operator * (const float scalaire, Quaternion<T> quaternion){
				return Quaternion<T>(scalaire*quaternion.elts);
			}

			// Multiplie le quaternion avec un scalaire et lui réaffecte le résultat
			void operator *= (const float scalaire){
				(*this) = (*this)*scalaire;
			}

			// Multiplie le quaternion avec quaternion2 et lui réaffecte le résultat
			void operator *= (Quaternion<T> quaternion2){
				(*this) = (*this)*quaternion2;
			}

			// Additionne le quaternion avec un scalaire
			Quaternion<T> operator + (const float scalaire){
				return Quaternion<T>(scalaire+re(),im());
			}

			// Additionne le quaternion avec quaternion2
			Quaternion<T> operator + (Quaternion<T> quaternion2){
				return Quaternion<T>(re()+quaternion2.re(),im()+quaternion2.im());
			}

			// Additionne un scalaire avec le quaternion
			friend Quaternion<T>  operator + (const float scalaire, Quaternion<T> quaternion){
				return Quaternion<T>(scalaire+quaternion.re(),quaternion.im());
			}

			// Additionne le quaternion avec un scalaire et lui réaffecte le résultat
			Quaternion<T> operator += (const float scalaire){
				(*this) = (*this) + scalaire;
			}

			// Additionne le quaternion avec quaternion2 et lui réaffecte le résultat
			Quaternion<T> operator += (Quaternion<T> quaternion2){
				(*this) = Quaternion<T>(re()+quaternion2.re(),im()+quaternion2.im());
			}

			// Retourne le résultat du quaternion - scalaire
			Quaternion<T> operator - (const float scalaire){
				return Quaternion<T>(scalaire-re(),im());
			}

			// Retourne le résultat du quaternion - quaternion2
			Quaternion<T> operator - (Quaternion<T> quaternion2){
				return Quaternion<T>(re()-quaternion2.re(),im()-quaternion2.im());
			}

			// Retourne le résultat du scalaire - le quaternion
			friend Quaternion<T>  operator - (const float scalaire, Quaternion<T> quaternion){
				return Quaternion<T>(scalaire-quaternion.re(),quaternion.im());
			}

			// Retourne l'inverse du quaternion
			Quaternion<T> operator- (){
				return Quaternion<T>(-re(),-im());
			}
			
	};

	template <typename T>
	class Sphere;

	// Point ayant N coordonnés de type T
	template<typename T, size_t N>
	class Point {
		private:
			Vector<T,N> coords;
		public:

			// Constructeur
			Point(){}

			// Constructeur
			Point(Vector<T,N> coords) : coords(coords) {}

			// Constructeur
			Point(std::initializer_list<T> list){
				coords = Vector<T,N>(list);
			}

			// Constructeur avec un array en param
			Point(T list[N]){
				coords = Vector<T,N>(list);
			}

			// Retourne la ième coordonnées du point
			T at(const int i){
				return coords.at(i);
			}

			//Retourne les coordonnées du point
			Vector<T,N> getCoords(){
				return coords;
			}
	
			// Retourne vrai si le point contient une valeur invalide
			bool is_null(){
				return coords.is_null();
			}
		
			// Retourne vrai si le point est derrière le plan passé en argument
			bool behind(Plane<T> &plane){
				//cout << "VALEUR: " << ((plane.getVector().to_unit())*(Vector<float,3>({coords[0],coords[1],coords[2]})))+plane.getD() << endl;
				return ((plane.getVector().to_unit())*(Vector<float,3>({coords[0],coords[1],coords[2]})))+plane.getD() < 0;
			}

			// Retourne la distance du point avec un plan
			float dist(Plane<T> &plane){
				return abs(((plane.getVector().to_unit())*(Vector<float,3>({coords[0],coords[1],coords[2]})))+plane.getD());
			}

			// Retourne vrai si le point est à l'intérieur de la sphère sphere
			bool inside(const Sphere<T> &sphere);

			// Retourne une direction qui représente le vecteur entre le point et point2
			Direction<T,N> length_to(Point<T,N> &point2){
				T res[N];
				for(int i=0;i<N;i++){
					res[i]=point2[i]-coords[i];
				}
				return Direction<T,N>(res);
			}


			// Retourne un nouveau point qui correspond a la rotation du point par le quaternion
			Point<T,N> rotate(Quaternion<T> quaternion){
				T list[N+1];
				list[0] = 0.0f;
				for(int i=1;i<N;i++){
					list[i]=coords[i];
				}
				Point<T,N+1> p(list);
				Quaternion<T> res = quaternion*p*quaternion.conjugate();
				
			}

			// Retourne le produit scalaire du point et d'un plan
			T operator * (Plane<T> &plane){
				if(N != 4){
					throw runtime_error("Le point doit être en 4D");
				}
				return plane.getConstantes() * coords;
			}

			// Retourne le résultat d'un point + direction
			Point<T,N> operator + (const Direction<T,N> &dir){
				return Point<T,N>(coords + dir.getCoords());
			}

			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const Point<T,N> &point){
				s << point.coords;
				return s;
			}

			// Adresse la ième coordonnée du point et permet l'affectation.
			T &operator [] (const int i){
				return this->coords[i];
			}

			// Retourne le point avec une dimension 4
			Point<T,4> get4D(){
				T list[4];
				for(int i=0;i<4;i++){
					if(i < N){
						list[i]=coords[i];
					} else {
						list[i] = 1;
					}
				}
				Vector<T,4> coords(list);
				return Point<T,4>(coords);
			}

			// Retourne le point avec une dimension 3
			Point<T,3> get3D(){
				T list[3];
				for(int i=0;i<3;i++){
					if(i < N){
						list[i]=coords[i];
					} else {
						list[i] = 1;
					}
				}
				Vector<T,3> coords(list);
				return Point<T,3>(coords);
			}
	};

	// Sphère
	template <typename T>
	class Sphere {
		private:
			T rayon;
			Point<T,3> center;
			
		public:
			//Constructeur
			Sphere(){}

			// Constructeur
			Sphere(Point<T,3> center, T rayon) : rayon(rayon), center(center) {}

			// Retourne vrai si la sphère contient des valeurs invalides
			bool is_null(){
				return center.is_null() || isnan(rayon);
			}

			// Retourne vrai si la sphère est derrière le plan
			bool behind(Plane<T> plane){
				return center.behind(plane) && abs(((plane.getVector().to_unit())*center.getCoords())+plane.getD()) >= rayon;
			}

			T getRayon() const {
				return rayon;
			}

			Point<T,3> getCenter() const {
				return center;
			}			

			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const Sphere<T> &sphere){
				s << sphere.center << " " << sphere.rayon;
				return s;
			}
			
	};

	// Retourne vrai si le point est à l'intérieur de la sphère sphere
	template <typename T, size_t N>
	bool Point<T,N>::inside(const Sphere<T> &sphere){
		Direction<T,N> dir = sphere.getCenter().length_to(*this);
		return  dir.norm() < sphere.getRayon();
	}

	// Rectangle
	template<typename T>
	class Rectangle {
		private:
			Point<T,2> points[4];

		public:
			// Constructeur
			Rectangle(Point<T,2> points[4]) : points(points) {};

			// Retourne vrai si le rectangle contient des valeurs invalides
			bool is_null(){
				for(int i=0;i<4;i++){
					if(points[i].is_null()){
						return 1;
					}
				}
				return 0;
			}
			
			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const Rectangle<T> &rectangle){
				s << "[" << rectangle.points[0] << ", " << rectangle.points[1] << ", " << rectangle.points[2] << ", " << rectangle.points[3] << "]";
				return s;
			}
	};

	// Triangle
	template<typename T>
	class Triangle {
		private:
			Point<T,3> sommets[3];
		public:

			// Constructeur
			Triangle(Point<T,3> sommets[3]){
				for(int i=0;i<3;i++){
					this->sommets[i] = sommets[i];
				}
			};

			// Retourne l'aire du triangle
			T area(){

				Direction<T,3> dir1 = sommets[1].length_to(sommets[0]);
				Direction<T,3> dir2 = sommets[2].length_to(sommets[0]);
				Direction<T,3> product = dir1.cross(dir2);
				return (1.f/2.f)*product.norm();
			}

			// Retourne vrai si le triangle contient des valeurs invalides
			bool is_null(){
				for(int i=0;i<3;i++){
					if(sommets[i].is_null()){
						return 1;
					}
				}
				return 0;
			}

			// Retourne le sommet 0
			Point<T,3> get_p0(){
				return sommets[0];
			}

			// Retourne le sommet 1
			Point<T,3> get_p1(){
				return sommets[1];
			}

			// Retourne le sommet 2
			Point<T,3> get_p2(){
				return sommets[2];
			}

			// Retourne la normale du triangle
			Vector<T,3> getNorm(){
				return (sommets[1].getCoords()-sommets[0].getCoords()).cross((sommets[2].getCoords()-sommets[0].getCoords())).to_unit();
			}

			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const Triangle<T> &triangle){
				s << "[" << triangle.sommets[0] << ", " << triangle.sommets[1] << ", " << triangle.sommets[2] << "]";
				return s;
			}
	};

	// Segment de droite
	template<typename T, size_t N>
	class LineSegment {
		private:
			Point<T,N> pt1;
			Point<T,N> pt2;

		public:
			// Constructeur
			LineSegment(){}
			
			// Constructeur prenant 2 points
			LineSegment(Point<T,N> pt1, Point<T,N> pt2) : pt1(pt1), pt2(pt2) {};

			// Retourne le point de départ du segment
			Point<T,N> get_begin(){
				return pt1;
			}
	
			// Retourne le point de fin du segment
			Point<T,N> get_end(){
				return pt2;
			}

			// Retourne le coefficient d'intersection entre le segment et un plan
			T inter_coef(Plane<T> &plane){
				if(N != 4){
					throw runtime_error("Les points doivent être 4D");
				}
				Direction<T,N> dir = pt1.length_to(pt2);
				T denominator = (dir * plane);
				
				if(!denominator){
					throw out_of_range("Valeur Nan");
				}
				return -((pt1 * plane) / denominator);
			}

			// Retourne le point d'intersection entre le segment et un plan
			Point<T,N> inter(Plane<T> &plane){

				try{
					float coef = inter_coef(plane);
					if(coef <= 1 && coef >= 0){
						return pt1 + (pt1.length_to(pt2) * coef);
					}
					throw out_of_range("Pas de point d'intersection");
				} catch(out_of_range const &e){
					if(pt1 * plane == 0){
						return pt1;
					}
					throw out_of_range("Pas de point d'intersection");
				}
				
			}

			// Retourne vrai si le segment contient une valeur invalide
			bool is_null(){
				return pt1.is_null() || pt2.is_null();
			}

			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const LineSegment<T,N> &segment){
				s << "( " << segment.pt1 << " , " << segment.pt2 << " )";
				return s;
			}
	};

	// Classe Transformation
	class Transformation{
		private:
			Matrix<float,4,4> matrix;
			float scal;
		public:
			// Constructeur
			Transformation(){
				matrix = Identity44r;
			}

			// Constructeur avec la matrice de transformation en param
			Transformation(Matrix<float,4,4> matrix) : matrix(matrix) {
			}

			// Constructeur pour créer la matrice de projection (n: distance entre la caméra et le near plan, f: distance entre la caméra et le far plan, r: longueur 
			Transformation(float n, float f, float r, float l, float b, float t){

				matrix[0][0] = (2*n)/(r-l);
				matrix[0][1] = 0;
				matrix[0][2] = (r+l)/(r-l);
				matrix[0][3] = 0;
				matrix[1][0] = 0;
				matrix[1][1] = (2*n)/(t-b);
				matrix[1][2] = (t+b)/(t-b);
				matrix[1][3] = 0;
				matrix[2][0] = 0;
				matrix[2][1] = 0;
				matrix[2][2] = -((f+n)/(f-n));
				matrix[2][3] = -((2*n*f)/(f-n));
				matrix[3][0] = 0;
				matrix[3][1] = 0;
				matrix[3][2] = -1;
				matrix[3][3] = 0;
			}
			
			// Constructeur: fait une rotation avec un quaternion
			Transformation(Quaternion<float> quaternion){
				matrix = Identity44r;
				float t = sin((quaternion.getAngle()*(M_PI / 180))/2.f);
				float w = quaternion.re();
				float x = t * quaternion.getAxis()[0];
				float y = t * quaternion.getAxis()[1];
				float z = t * quaternion.getAxis()[2];

				matrix[0][0] = 1-2*pow(y,2)-2*pow(z,2);
				matrix[0][1] = 2*x*y-2*w*z;
				matrix[0][2] = 2*x*z+2*w*y;
				matrix[1][0] = 2*x*y+2*w*z;
				matrix[1][1] = 1-2*pow(x,2)-2*pow(z,2);
				matrix[1][2] = 2*y*z-2*w*x;
				matrix[2][0] = 2*x*z-2*w*y;
				matrix[2][1] = 2*y*z+2*w*x;
				matrix[2][2] = 1-2*pow(x,2)-2*pow(y,2);
			}
		
			// Constructeur: fait une rotation avec angle et direction
			Transformation(const float angle, Direction<float,3> direction){
				(*this) = Transformation(Quaternion<float>(angle,direction));
			}

			// Constructeur: fait une translation ou mise à l'échelle a partir des axes donnés en param
			Transformation(Direction<float,3> axe, const bool forScale){
				if(!forScale){
					matrix = Identity44r;
					for(int i=0;i<3;i++){
						matrix[i][3] = axe.at(i);
					}
				} else {
					matrix = Identity44r;
					for(int i=0;i<3;i++){
						matrix[i][i] *= axe.at(i);
					}
					scal = axe[0];
				}
			}

			// Retourne la concaténation de deux transformations
			Transformation concat(Transformation transformation2){
				return Transformation((transformation2.getMatrix() * matrix));
			}

			// Retourne la matrice de transformation
			Matrix<float,4,4> getMatrix(){
				return matrix;
			}

			Transformation getInverse(){
				return Transformation(matrix.inverse());
			}

			Transformation getTranspose(){
				return Transformation(matrix.transpose());
			}

			// Retourne le quaternion correspondant à une rotation
			Quaternion<float> to_quat(){
				float qw,qx,qy,qz;
				float tr = matrix[0][0] + matrix[1][1] + matrix[2][2];
				if (tr > 0) { 
					float S = sqrt(tr+1.0) * 2;
					qw = 0.25 * S;
					qx = (matrix[2][1] - matrix[1][2]) / S;
					qy = (matrix[0][2] - matrix[2][0]) / S; 
					qz = (matrix[1][0] - matrix[0][1]) / S; 

				} else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2])) { 
					float S = sqrt(1.0 + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2; 
					qw = (matrix[2][1] - matrix[1][2]) / S;
					qx = 0.25 * S;
					qy = (matrix[0][1] + matrix[1][0]) / S; 
					qz = (matrix[0][2] + matrix[2][0]) / S; 

				} else if (matrix[1][1] > matrix[2][2]) { 
					float S = sqrt(1.0 + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2;
					qw = (matrix[0][2] - matrix[2][0]) / S;
					qx = (matrix[0][1] + matrix[1][0]) / S;
					qy = 0.25 * S;
					qz = (matrix[1][2] + matrix[2][1]) / S;

				} else { 
					float S = sqrt(1.0 + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2;
					qw = (matrix[1][0] - matrix[0][1]) / S;
					qx = (matrix[0][2] + matrix[2][0]) / S;
					qy = (matrix[1][2] + matrix[2][1]) / S;
					qz = 0.25 * S;
				}

				return Quaternion<float>(Vector<float,4>({qw,qx,qy,qz}));
			}

			// Retourne un nouveau point qui correspond à la transformation d'un point
			Point<float,4> transform(Point<float,4> point){
				return Point<float,4>(matrix*point.getCoords());
			}

			// Retourne une nouvelle direction qui correspond à la transformation d'une direction
			Direction<float,4> transform(Direction<float,4> direction){
				return Direction<float,4>(matrix*direction.getCoords());
			}

			// Retourne une nouvelle sphère qui correspond à la transformation
			Sphere<float> transform(Sphere<float> sphere){
				return Sphere<float>(transform(sphere.getCenter().get4D()).get3D(),sphere.getRayon());
			}

			Triangle<float> transform(Triangle<float> triangle){
				Point<float,3> pts[3];
				pts[0] = transform(triangle.get_p0().get4D()).get3D();
				pts[1] = transform(triangle.get_p1().get4D()).get3D();
				pts[2] = transform(triangle.get_p2().get4D()).get3D();
				return Triangle<float>(pts);
			}

			// Retourne un nouveau plan correspondant à la transformation
			Plane<float> transform(Plane<float> plane){
				Vector<float,4> tmp= matrix.inverse().transpose()*plane.getConstantes();
				return Plane<float>(tmp);

				/*Vector<float,4> tmp({plane.getConstantes()[0]*plane.getConstantes()[3],
														plane.getConstantes()[1]*plane.getConstantes()[3],
														plane.getConstantes()[2]*plane.getConstantes()[3],1});

				Vector<float,4> tmp2({plane.getConstantes()[0],plane.getConstantes()[1],plane.getConstantes()[2],0});

				tmp = matrix*tmp;
				tmp2 = matrix.inverse().transpose()*tmp2;
				Vector<float,3> vec1({tmp[0],tmp[1],tmp[2]});
				Vector<float,3> vec2({tmp2[0],tmp2[1],tmp2[2]});
				return Plane<float>(Vector<float,4>({tmp2[0],tmp2[1],tmp2[2],vec1*vec2}));*/
			}

			// Surchage de l'opérateur pour les outputs
			friend ostream &operator << (ostream &s, const Transformation &transfo){
				s << transfo.matrix;
				return s;
			}
	};

}

#endif
