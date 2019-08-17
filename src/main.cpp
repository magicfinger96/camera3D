#include <iostream>
#include <fstream>
#include "libmatrix.h"
#include "libgeometry.h"
#include "apps.h"

using namespace libmatrix;
using namespace libgeometry;

// Tests réalisés pour le TP1
void test_TP1(){
	// vecteurs 
	Vector<float,3> vector01({10,10,20});
	Vector<float,3> vector00({5,13,11});
	Vector<float,3> vector02({5,30,2});
	cout << vector01.at(2) << endl;
	Vector<float,3> vector03 = vector01.cross(vector02);
	cout << vector03.at(0) << endl;
	cout << vector02.is_ortho(vector01) << endl;
	cout << vector01.is_null() << endl;
	Vector<float,2> vector04({-4.0/sqrt(80.0),-8.0/sqrt(80.0)});
	cout << vector04.norm() << endl;
	cout << vector04.is_unit() << endl;
	Vector<float,3> vector05 = vector01.to_unit();
	cout << vector05.at(0) << " " << vector05.at(1) << " " << vector05.at(2) << endl;
	vector01[0] = 6;
	cout << vector01 << endl;
	Vector<float,3> vec06 = vector01 + vector02;
	cout << vec06 << endl;
	vector00+=vector01;
	cout << vector00 << endl;

	Vector<float,3> vector010({19,30,20});
	Vector<float,3> vector011({5,15,11});
	cout << (vector010 - vector011) << endl;

	vector010-=vector011;
	cout << vector010 << endl;
	cout << (-vector010) << endl;
	cout << (vector010 * vector011) << endl;
	cout << (vector010 * 10) << endl;
	cout << (10 * vector010) << endl;
	vector010*=2;
	cout << vector010 << endl;
	
	//matrices
	Vector<float,4> vec1({0.000195313,0,0,0});
	Vector<float,4> vec2({0,0.000260417,0,0});
	Vector<float,4> vec3({0,0,-1.0002,-0.20002});
	Vector<float,4> vec4({0,0,-1,0});
	Matrix<float,4,4> mat({vec1,vec2,vec3,vec4});
	Matrix<float,4,4> mat2 = mat.inverse();
	/*Matrix<float,3,3> mat3 = mat.transpose();
	cout << mat3 << endl;
	cout << mat.is_ortho() << endl;

	Matrix<float,3,3> mat4({vec1,vec2,vec3});
	cout << (mat == mat4) << endl;

	Vector<float,3> vec4({1,0,0});
	Vector<float,3> vec5({0,1,0});
	Vector<float,3> vec6({0,0,-1});
	Matrix<float,3,3> mat5({vec4,vec5,vec6});
	cout << mat5.is_ortho() << endl;

	cout << (mat5 * 2.0) << endl;

	mat5*=2.0;
	cout << mat5 << endl;

	cout << (mat5 + mat) << endl;
	mat5 += mat;
	cout << mat5 << endl;

	float f = 2.0;
	cout << (f * mat5) << endl;

	cout << "les mult:" << endl;
	cout << mat5 << endl << mat << endl;
	cout << (mat5 * mat) << endl;
	mat5*=mat;
	cout << mat5 << endl;

	Vector<float,3> vec100({0,3,5});
	Vector<float,3> vec200({5,5,2});
	Matrix<float,2,3> mat100({vec100,vec200});
	Vector<float,3> vec300({3,4,3});
	cout << mat100 << endl;
	cout << vec300 << endl;
	cout << (mat100 * vec300) << endl;
	cout << dot(mat100,vec300) << endl;

	Vector<float,2> vec0102({1,1});
	Vector<float,2> vec0202({2,3});
	Vector<float,2> vec0302({1,-1});
	Matrix<float,3,2> mat0202({vec0102,vec0202,vec0302});

	Vector<float,4> vec0203({0,1,-1,-2});
	Vector<float,4> vec0303({-3,-2,0,1});
	Matrix<float,2,4> mat0302({vec0203,vec0303});

	cout << (mat0202 * mat0302) << endl;

	Vector<float,3> vec202({2,3,2});
	Vector<float,3> vec302({1,-1,8});
	Matrix<float,2,3> mat202({vec202,vec302});
	Vector<float,2> vec305({1,4});
	cout << (vec305 * mat202) << endl;

	Vector<float,3> vector1({10,10,20});
	Vector<float,3> vector0({5,13,11});
	cout << (vector1.cross(vector0)) << endl;
	cout << cross(vector1,vector0) << endl;
	cout << Identity33r << endl;*/
}

// Tests réalisés pour le TP2
void test_TP2(){
	Vector<float,4> vec({1,2,22,4});
	Quaternion<float> q(vec);
	cout << q << endl;
	cout << q.inverse() << endl;

	Point<float,3> pts[3];
	pts[1] = Point<float,3>({2.0,1.5,1.0});
	pts[2] = Point<float,3>({6.1,1.4,3.0});
	pts[0] = Point<float,3>({5.0,1.9,2.4});

	Triangle<float> triangle(pts);
	cout << triangle.area() << endl;

	Point<float,3> pt1({4.5,5,3.5});
	Sphere<float> sphere(Point<float,3>({4.92,5.14,2.6}),2.f);
	cout << pt1.inside(sphere) << endl;

	Point<float,3> pt2({5.9,3.65,3.87});
	cout << pt2.inside(sphere) << endl;
	Plane<float> plane(Vector<float,4>({2.0,3.0,5.0,2.0}));
	cout << pt2.behind(plane) << endl;

	Point<float,3> pt3({3.33,-6.65,-1.37});
	cout << pt3.behind(plane) << endl;

	Point<float,4> pt4({-0.4,-2.2,0,1});
	Point<float,4> pt5({-0.36,-1.27,0.91,1});
	LineSegment<float,4> segment(pt5,pt4);
	cout << " Le point d'intersection: " << segment.inter(plane) << endl;
	cout << " Le coef d'intersection: " << segment.inter_coef(plane) << endl;

	try{
		Point<float,4> pt6({-0.36,-1.27,0.37,1});
		LineSegment<float,4> segment2(pt4,pt6);
		cout << " Le point d'intersection: " << segment2.inter(plane) << endl;
		cout << " Le coef d'intersection: " << segment2.inter_coef(plane) << endl;
	} catch(out_of_range const &e){
		cout << "Pas de point d'intersection !" << endl;
	}

		Point<float,4> pt7({-2.76,-0.86,1.22,1});
		Point<float,4> pt8({-0.37,-1.47,0.63,1});
		LineSegment<float,4> segment2(pt7,pt8);
		cout << " Le point d'intersection: " << segment2.inter(plane) << endl;

		Sphere<float> sphere2(Point<float,3>({-8.19,0.65,-4.64}),2.f);
		cout << "La sphère est derrière: " << sphere2.behind(plane) << endl;

		Sphere<float> sphere3(Point<float,3>({-28.66,20.3,22.75}),2.f);
		cout << "La sphère n'est pas derrière: " << sphere3.behind(plane) << endl;

		Sphere<float> sphere4(Point<float,3>({-52.68,7.58,17.21}),2.f);
		cout << "La sphère n'est pas derrière: " << sphere4.behind(plane) << endl;

		Sphere<float> sphere5(Point<float,3>({-52.64,10.83,17.21}),2.f);
		cout << "La sphère n'est pas derrière: " << sphere5.behind(plane) << endl;

		Sphere<float> sphere6(Point<float,3>({-53.62,-3.94,17.21}),2.f);
		cout << "La sphère est derrière: " << sphere6.behind(plane) << endl;

		Quaternion<float> q2(20,Direction<float,3>(Vector<float,3>({4.f,2.f,3.f})));
		cout << q2 << endl;
		cout << q2.getAngle() << endl;
		cout << q2.getAxis() << endl;

		Transformation t(q2);
		cout << t << endl;

		Transformation t2(20,Direction<float,3>(Vector<float,3>({4.f,2.f,3.f})));
		cout << t2 << endl;

		Transformation t3(Direction<float,3>(Vector<float,3>({4.f,2.f,3.f})),1);
		cout << t3 << endl;

		Transformation t4(Direction<float,3>(Vector<float,3>({4.f,2.f,3.f})),0);
		cout << t4 << endl;

		cout << t3.concat(t4) << endl;
		Transformation t5;
		cout << (t5.transform(Point<float,4>(Vector<float,4>({2,4,5,6})))) << endl;
		cout << (t5.transform(Direction<float,4>(Vector<float,4>({4.f,2.f,3.f,2.f})))) << endl;
		
		cout << "Vers quaternion " << t.to_quat() << endl;
		cout << "Son axe " << t.to_quat().getAxis() << endl ;		
		cout << "Sa rotation " << t.to_quat().getAngle() << endl << endl;

		cout << t5.transform(sphere) << endl;
}

void test_TP3(){
	Camera camera;
	Point<float,3> pts[3];
	pts[1] = Point<float,3>({2.0,1.5,1.0});
	pts[2] = Point<float,3>({6.1,1.4,3.0});
	pts[0] = Point<float,3>({5.0,1.9,2.4});
	Triangle<float> triangle(pts);
	cout << "Angle: " << camera.sees(triangle) << endl;
}

// Ouvre un fichier au format .geo et insère l'objet dans la scène
void load_geo_file(const char *file, Scene &scene){
	ifstream myfile(file);
	if(myfile.is_open()){
		while(!myfile.eof()){
			Object3D object;
			int nbLignes;
			myfile >> nbLignes;
			if(!myfile.good()){
				break;
			}

			for(int i=0;i<nbLignes;i++){
				float val1, val2, val3;
				myfile >> val1 >> val2 >> val3;
				object.add_sommet(Point<float,4>(Vector<float,4>({val1,val2,val3,1})));
			}
			
			myfile >> nbLignes;
			for(int i=0;i<nbLignes;i++){
				int som1,som2,som3;
				myfile >> som1 >> som2 >> som3;
				object.add_face(som1-1,som2-1,som3-1);
			}
			scene.addObject(object);
		}
		myfile.close();
	} else {
		cout << "Impossible d'ouvrir le fichier !" << endl;
	}
}

int main(int argc, const char * argv[]){
	//test_TP1();
	//test_TP2();
	//test_TP3();

	Camera camera(1024,768, Point<float,4>(Vector<float,4>({0.0005,0,0,1})),Quaternion<float>(90,Direction<float,3>(Vector<float,3>({0,1,0}))).to_norm(),120.f);
	Scene scene(camera);
	for(int i=1;i<argc;i++){
		load_geo_file(argv[i],scene);
	}
	scene.launchLoop();

	return 1;
}
