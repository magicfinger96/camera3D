#ifndef matrix_andrea
#define matrix_andrea
#include <iostream>
#include <initializer_list>
#include <exception>
#include <math.h>
#include <string>
#define EPSILON 0.0001

//quel type de retour pr norme ? float ou double ? type T


using namespace std;

namespace libmatrix {

	//Vecteur de type T et de taille N
	template<typename T, size_t N>
	class Vector {
		private:
			T elements[N];
		public:
			//Constructeur
			Vector(){};
			
			//Constructeur initialisé avec une liste d'éléments
			Vector(T list[N]){
				for(int i=0;i<N;i++)
				{
					  elements[i]= list[i];
				}
			};

			// Constructeur initialisé avec un initializer
			Vector( std::initializer_list<T> list)
			{
				int i=0;
				for( T elem : list )
				{
					  elements[i++]= elem;
				}
			}

			// Retourne le ième élément du vecteur
			T at(const int i) const {
				if(i < 0 || i >= N)
					throw runtime_error("Mauvais indice");
				return this->elements[i];
			}

			// Fait un cross entre le vecteur et vector2
			Vector<T,N> cross(Vector<T,N> vector2){
				if(N < 3)
					throw runtime_error("Le vecteur possède moins de 3 coordonnées");
				Vector<T,N> result({this->elements[1]*vector2.at(2)-this->elements[2]*vector2.at(1),this->elements[2]*vector2.at(0)-this->elements[0]*vector2.at(2),this->elements[0]*vector2.at(1)-this->elements[1]*vector2.at(0)});
				return result;
			}
			
			// Retourne vrai si le vecteur est orthogonal
			bool is_ortho(Vector<T,N> vector2){
				float resultat = 0;
				for(int i=0;i<N;i++){
					resultat += this->elements[i]*vector2.at(i);
				}
				return resultat == 0;
			}
	
			// Retourne vrai si le vecteur contient une valeur invalide
			bool is_null(){
				const string i = "i";
				const string d = "d";
				const string f = "f";
				const string l = "l";

				for(int k=0;k<N;k++){
					if((i.compare(typeid(T).name()) != 0 && d.compare(typeid(T).name()) != 0  && f.compare(typeid(T).name()) != 0 && l.compare(typeid(T).name()) != 0) || isnan(this->elements[k]))
						return true;
				}
				return false;
			}

			// Retourne la norme du vecteur
			T norm(){
				float res = 0;
				for(int i=0;i<N;i++){
					res+= pow(this->elements[i],2);
				}
				return sqrt(res);
			}

			// Retourne vrai si le vecteur est unitaire
			bool is_unit(){
				return (this->norm()-1 < EPSILON);
			}

			// Retourne une copie du vecteur normalisé
			Vector<T,N> to_unit(){
				T n = this->norm();
				T list[N];
				for(int i=0;i<N;i++){
					list[i]=this->elements[i]/n;
				}

				Vector<T,N> v(list);
				return v;
			}

			// Affiche le vecteur dans la console
			friend ostream &operator << (ostream &s, Vector<T,N> vector){
				s << "(";
				for(int i=0;i<N-1;i++){
					s << vector.at(i) << " ";
					
				}
				s << vector.at(N-1) << ")";
				return s;
			}

			// Retourne l'élement à la ième position du vecteur, affectation possible
			T &operator [] (const int i){
				return this->elements[i];
			}

			// Additionne le vecteur + vector et retourne le résultat
			Vector<T,N> operator + (Vector<T,N> vector){
				T list[N];
				for(int i=0;i<N;i++){
					list[i]=vector.at(i)+this->elements[i];
				}
				Vector<T,N> res(list);
				return res;
			}

			// Affecte au vecteur le résultat du vecteur + vector
			void operator += (Vector<T,N> vector){
				Vector<T,N> res = (*this)+vector;
				for(int i=0;i<N;i++){
					this->elements[i] = res.at(i);
				}
			}

			// Retourne le résultat de la soustraction du vecteur - vector
			Vector<T,N> operator - (Vector<T,N> vector){
				T list[N];
				for(int i=0;i<N;i++){
					list[i]=this->elements[i]-vector.at(i);
				}
				Vector<T,N> res(list);
				return res;
			}

			// Affecte au vecteur sa soustraction avec vector
			void operator -= (Vector<T,N> vector){
				Vector<T,N> res = (*this)-vector;
				for(int i=0;i<N;i++){
					this->elements[i] = res.at(i);
				}
			}

			// Retourne l'inverse du vecteur
			Vector<T,N> operator- (){
				Vector<T,N> res;
				for(int i=0;i<N;i++){
					res[i] = -this->elements[i];
				}
				return res;
			}

			// Fait le produit scalaire du vecteur avec vector
			float operator* (Vector<T,N> vector){
				float res = 0;
				for(int i=0;i<N;i++){
					res+=this->elements[i]*vector.at(i);
				}
				return res;
			}

			// Multiplie le vecteur par un scalaire
			Vector<T,N> operator* (const float scalaire){
				Vector<T,N> res;
				for(int i=0;i<N;i++){
					res[i] =this->elements[i]*scalaire;
				}
				return res;
			}

			//Multiplie le vecteur par un scalaire et lui affecte le résultat
			void operator*= (const float scalaire){
				Vector<T,N> res = (*this)*scalaire;
				for(int i=0;i<N;i++){
					this->elements[i] = res.at(i);
				}
			}

			// Retourne vrai si vector et le vecteur sont identiques
			bool operator== (Vector<T,N> vector){
				for(int i=0;i<N;i++){
					if(this->elements[i] != vector.at(i)){
						return false;
					}
				}
				return true;
			}
	};

	//Matrice de type T ayant M lignes N colonnes
	template<typename T, size_t M, size_t N>
	class Matrix {
		private:
			Vector<T,N> elements[M];
		public:
			// Constructeur
			Matrix<T,M,N>(){};

			// Constructeur: initialise avec un tableau
			Matrix(Vector<T,N> list[M]){
				for(int i=0;i<M;i++)
				{
					  elements[i]= list[i];
				}
			};

			// Constructeur: initialise avec un initializer
			Matrix( std::initializer_list<Vector<T,N>> list)
			{
				int i=0;
				for( Vector<T,N> elem : list )
				{
					  elements[i++]= elem;
				}
			}

			// Retourne l'élément à la position i j de la matrice
			T at(const int i, const int j){
				if(i < 0 || i >= M || j < 0 || j >= N)
					return 0;
				return this->elements[i][j];
			}

			// Retourne la ième ligne de la matrice
			Vector<T,N> at(const int i){
				if(i < 0 || i >= M)
					throw runtime_error("Mauvais indice");
				return this->elements[i];
			}

			// Retourne l'inverse de la matrice
			Matrix<T,M,N> inverse(){
				Matrix<T,M,N> res = (*this);
				Vector<T,N> list[M];
				for(int i=0;i<M;i++){
					T pts[N];
					for(int j=0;j<N;j++){
						if(j==i){
							pts[j]=1;
						} else {
							pts[j]=0;
						}
					}
					list[i]= Vector<T,N>(pts);
				}
				Matrix<T,M,N> id(list);

				for(int i=0;i<N;i++){
					int max = i;
					for(int k=i;k<M;k++){
						if(fabs(res[k][i]) > fabs(res[max][i])){
							max = k;
						}
					}

					if(fabs(res[max][i]) == 0.0f){
						throw out_of_range("Pas inversible");
					}

					if(max != i){
						Vector<T,N> tmp = res[i];
						res[i] = res[max];
						res[max] = tmp;

						Vector<T,N> tmp2 = id[i];
						id[i] = id[max];
						id[max] = tmp2;
					}

					float val = res[i][i];
					res[i] *= (1/val);
					id[i] *= (1/val);

					for(int p =0;p<M;p++){
						if(p != i){
							float val = -res[p][i];
							res[p]+=(res[i]*val);
							id[p]+=(id[i]*val);
						}
					}
				}
				return id;	
			}

			// Retourne vrai si la matrix contient des valeurs invalides
			bool is_null(){
				for(int i=0;i<M;i++){
					if(this->elements[i].is_null()){
						return true;
					}
				};
				return false;
			}

			// Retourne vrai si la matrice est orthogonale, faux sinon
			bool is_ortho(){
				return this->inverse() == this->transpose();
			}

			// Retourne la transposée de la matrice
			Matrix<T,N,M> transpose(){
				Matrix<T,N,M> res;
				for(int i=0;i<N;i++){
					for(int j=0;j<M;j++){
						res[i][j]=(*this)[j][i];
					}
				}
				return res;
			}

			// Affiche la matrice dans la console
			friend ostream &operator << (ostream &s, Matrix<T,M,N> matrix){
				for(int i=0;i<M;i++){
					s << "[";
					for(int j=0;j<N;j++){
						s << matrix.at(i,j);
						if(j<N-1){
							s << " ";
						}
					}
					s << "]" << endl;
				}	
			
				return s;
			}
		
			// Retourne la ieme ligne de la matrice (affectation possible)
			Vector<T,N> &operator [] (const int i){
				return this->elements[i];
			}

			// Retourne vrai si matrix est égale à la matrice
			bool operator== (Matrix<T,M,N> matrix){
				for(int i=0;i<N;i++){
					if(!(this->elements[i] == matrix.at(i))){
						return false;
					}
				}
				return true;
			}

			// Retourne matrice + matrice
			Matrix<T,M,N> operator+ (Matrix<T,M,N> matrix){
				Matrix<T,M,N> res = matrix;
				for(int i=0;i<M;i++){
					res[i] += this->elements[i];
				}
				return res;
			}

			// Retourne le résultat de Matrice * scalaire
			Matrix<T,M,N> operator* (T scalaire){
				Matrix<T,M,N> res = (*this);
				for(int i=0;i<M;i++){
					res[i] *= scalaire;
				}
				return res;
			}

			// Retourne le résultat d'une multiplication matrice * matrice
			template<size_t M2>
			Matrix<T,M,M2> operator* (Matrix<T,N,M2> matrix){
				Matrix<T,M,M2> res;
				for(int i=0;i<M;i++){
					for(int j=0;j<M2;j++){
						res[i][j] = 0;
						for(int k=0;k<N;k++){
							res[i][j]+=(this->elements[i][k]*matrix.at(k,j));
						}
					}
				}
				return res;
			}

			// Retourne le résultat d'une multiplication matrice * vecteur
			Vector<T,M> operator* (Vector<T,N> vector){
				Vector<T,1> elemts[N];
				for(int i=0;i<N;i++){
					Vector<T,1> vec1({vector.at(i)});
					elemts[i]= vec1;
				}
				Matrix<T,N,1> mat(elemts);
				Matrix<T,M,1> res = (*this)*mat;
				Vector<T,M> vec;
				for(int i=0;i<M;i++){
					vec[i] = res[i][0];
				}
				return vec;
			}

			// matrice1 * matrice et affecte le résultat à matrice1
			void operator*= (Matrix<T,M,N> matrix){
				Matrix<T,M,N> res = (*this)*matrix;
				for(int i=0;i<M;i++){
					this->elements[i] = res[i];
				}
			}

			// matrice * scalaire et affecte le résultat à la matrice
			void operator*= (T scalaire){
				Matrix<T,M,N> res = (*this)*scalaire;
				for(int i=0;i<M;i++){
					this->elements[i] = res[i];
				}
			}

			// Matrice + Matrice et affecte le résultat à la 1ere matrice
			void operator+= (Matrix<T,M,N> matrix){
				Matrix<T,M,N> res = (*this)+matrix;
				for(int i=0;i<M;i++){
					this->elements[i] = res[i];
				}
			}
			
			
	};

	// Scalaire * vecteur
	template<typename T, size_t N>
	Vector<T,N> operator*(const float scalaire, Vector<T,N> vector){
		Vector<T,N> res;
		for(int i=0;i<N;i++){
			res[i]=vector[i]*scalaire;
		}
		return res;
	}

	// Scalaire * matrice
	template<typename T, size_t M, size_t N>
	Matrix<T,M,N> operator*(const T scalaire, Matrix<T,M,N> matrix){
		Matrix<T,M,N> res = matrix*scalaire;
		return res;
	}

	// vecteur * matrice
	template<typename T, size_t M, size_t N>
	Vector<T,M> operator* (Vector<T,N> vector, Matrix<T,N,M> matrix){
		Matrix<T,1,N> mat({vector});
		Matrix<T,1,M> res = mat * matrix;
		Vector<T,M> vec;
		for(int i=0;i<M;i++){
			vec[i] = res[0][i];
		}
		return vec;
	}

	// alias de l'opération *
	auto dot(auto op1, auto op2){
		return op1 * op2;
	}

	// alias de la méthode cross
	auto cross(auto op1, auto op2){
		return op1.cross(op2);
	}

	using Vec2i= Vector<int,2>;
	using Vec3i= Vector<int,3>;
	using Vec4i= Vector<int,4>;
	using Vec2r= Vector<float,2>;
	using Vec3r= Vector<float,3>;
	using Vec4r= Vector<float,4>;
	using Mat33r= Matrix<float,3,3>;
	using Mat44r= Matrix<float,4,4>;

	Vec2i zerovec2i({0,0});
	Vec3i zerovec3i({0,0,0});
	Vec4i zerovec4i({0,0,0,0});
	Vec2r zerovec2r({0.0,0.0});
	Vec3r zerovec3r({0.0,0.0,0.0});
	Vec4r zerovec4r({0.0,0.0,0.0,0.0});

	Mat33r Identity33r({Vec3r({1.0,0.0,0.0}),Vec3r({0.0,1.0,0.0}),Vec3r({0.0,0.0,1.0})});
	Mat44r  Identity44r({Vec4r({1.0,0.0,0.0,0.0}),Vec4r({0.0,1.0,0.0,0.0}),Vec4r({0.0,0.0,1.0,0.0}),Vec4r({0.0,0.0,0.0,1.0})});
}
#endif
