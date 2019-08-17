#ifndef _APPS_
#define _APPS_

#include <iostream>
#include <vector>
#include <exception>
#include <math.h>
#include "gui.h"
#include "libmatrix.h"
#include "libgeometry.h"

using namespace libmatrix;
using namespace libgeometry;
using namespace gui;

class Camera;

// Représente un frustum
class Frustum {
	private:
		friend Camera;
		Plane<float> planes[6]; // closePlane farPlane rightPlane leftPlane supPlane infPlane

	public:
		Frustum(){}
		Frustum(Plane<float> planes[6]) {
			for(int i=0;i<6;i++){
				this->planes[i] = planes[i];
			}
		}

		// Retourne le plan à la position i
		Plane<float> getPlane(int i){
			return planes[i];
		}

		// Met à jour le plan à la position i
		void setPlane(int i, Plane<float> plane){
			planes[i] = plane;
		}

		// Retourne vrai si le point passé en arg est en dehors du champ de vision
		bool outside(Point<float, 4> &point )  {
			return point.behind(planes[0]) || point.behind(planes[1]) || point.behind(planes[2]) || point.behind(planes[3]) || point.behind(planes[4]) || point.behind(planes[5]);
		}

		// Retourne vrai si la sphère passée en arg est complétement en dehors du champs de vision
		bool outside( Sphere<float> &sphere )  {
			return sphere.behind(planes[0]) || sphere.behind(planes[1]) || sphere.behind(planes[2]) || sphere.behind(planes[3]) || sphere.behind(planes[4]) || sphere.behind(planes[5]);
		}

		// Retourne l'intersection du segment avec le champ de vision (la partie visible), retourne une exception s'il n'y a pas de segment visible.
		LineSegment<float,4> inter(  LineSegment<float,4> &segment)  {
 			vector<Point<float,4>> pts;
			Point<float,4> pt_begin = segment.get_begin();
			Point<float,4> pt_end = segment.get_end();

			return segment;
			//cout << "INTER : " << endl;

			for(int i=0;i<6;i++){
				try {
					Point<float,4> pt = segment.inter(planes[i]);
					//cout << "le pt d'intersection de : " << planes[i] << " et " << segment << " est: "<< pt << endl;
					if(!outside(pt)){
						pts.push_back(pt);
					}
				} catch(const out_of_range& e){
					if(!outside(pt_begin) && !outside(pt_end)){
						pts.push_back(pt_begin);
						pts.push_back(pt_end);
					}
					//cout << "le pt d'intersection de : " << planes[i] << " et " << segment << " EXISTE PAS"<< endl;
					continue;
				}
			}

			//cout << "On a enregistré: " << pts.size() << " points d'intersection" << endl;

			if(pts.size() == 0){
				if(!outside(pt_begin) && !outside(pt_end)){
					return segment;
				} else {
					throw out_of_range("Pas de segment visible");
				}
			} else if(pts.size() == 1){
				if(!outside(pt_begin)){
					return LineSegment<float,4>(pt_begin,pts[0]);
				} else {
					return LineSegment<float,4>(pts[0],pt_end);
				}
			} else if(pts.size() == 2){
				return LineSegment<float,4>(pts[0],pts[1]);
			} else {
				throw out_of_range("Pas de segment visible");
				//throw runtime_error("Il ne devrait pas y avoir plus de 3 pts d'intersection");
			}
		}

		// Met à jour la position du champ de vision, où h est la résolution horizontale, v la résolution verticale et e la distance entre le plan de projection et la caméra.
		void update (float h, float v, float e){
			
		}
		
};

class Scene;

// Représente une caméra
class Camera {
	private:
		friend Scene;
		float width;
		float height;
		float movingSpeed;
		float changingOrientationSpeed;
		float zoomingSpeed;
		Point<float,4> position;
		Quaternion<float> orientation;
		Vector<float,3> actualMovingSpeed;
		Vector<float,3> actualChangingOrientationSpeed;
		Vector<float,3> actualZoomSpeed;
		float lengthToPlane;
		float focalLenght;
		Frustum *frustum;

	public:
		// Constructeur
		Camera(){
			frustum = nullptr;
		}

		// Destructeur
		~Camera(){
			if(frustum != nullptr){
				delete frustum;
			}
		}

		// Constructeur avec taille, frustum, direction et position en paramètre
		Camera(float width, float height, Point<float,4> position, Quaternion<float> orientation, float angle) : width(width), height(height), position(position), orientation(orientation) {

			movingSpeed = 0;
			changingOrientationSpeed = 0;
			zoomingSpeed = 0;

			focalLenght = 1/tan((angle*M_PI/ 180.0)/2);
			float a = height/width;
			float n = 0.1;
			float f = 1000;
			float e = focalLenght;

			Plane<float> planes[6] = {Plane<float>(Vector<float,4>({0,0,-1,-n})),Plane<float>(Vector<float,4>({0,0,1,f})), Plane<float>(Vector<float,4>({e,0,-1,0})), Plane<float>(Vector<float,4>({-e,0,-1,0})), Plane<float>(Vector<float,4>({0,e,-a,0})), Plane<float>(Vector<float,4>({0,-e,-a,0}))};

			frustum = new Frustum(planes);
		}

		// Met a jour la distance focale
		void updateFocalDistance(){

			float e = focalLenght;
			float a = height/width;
			float n = 0.1;
			float f = 1000;

			Plane<float> planes[6] = {Plane<float>(Vector<float,4>({0,0,-1,-n})),Plane<float>(Vector<float,4>({0,0,1,f})), Plane<float>(Vector<float,4>({e,0,-1,0})), Plane<float>(Vector<float,4>({-e,0,-1,0})), Plane<float>(Vector<float,4>({0,e,-a,0})), Plane<float>(Vector<float,4>({0,-e,-a,0}))};

			frustum = new Frustum(planes);
		}

		// Retourne la transformation qui correspond au point de vue de la caméra
		Transformation get_transform() {
			Transformation rotation(orientation);
			Transformation translation(Direction<float,3>(position.get3D().getCoords()),0);
			return rotation.getInverse().concat(translation.getInverse());
		}

		// Retourne vrai si la sphère passée en argument est en dehors du champs de vision de la caméra.
		bool outside_frustum( Sphere<float> &sphere) {
			return frustum->outside(sphere);
		}

		// Retourne vrai si la caméra voie la face triangulaire passée en argument
		bool sees(Triangle<float> &triangle) {
			Vector<float,3> cameraVec = orientation.getAxis().getCoords();
			return triangle.getNorm()*cameraVec < 0;
		}

		// Retourne la partie visible du segment passé en argument
		LineSegment<float,4> visible_part (LineSegment<float,4> &segment) {
			return frustum->inter(segment);
		}

		// Met a jour la position et orientation de la caméra
		void update(){
			for(int i = 0;i<6;i++){
				Plane<float> tmp = get_transform().transform(frustum->planes[i]);
				frustum->setPlane(i,tmp);
			}
		}

};

// Représente un objet 3D
class Object3D {
	private:
		friend Scene;
		string name;
		Point<float,4> position;
		vector<Point<float,4>> sommets;
		vector<Triangle<float>> mesh;

	public:
		// Constructeur
		Object3D(){}
		
		// Retourne la bounding sphere
		Sphere<float> bsphere() {
			float moyX = 0;
			float moyY = 0;
			float moyZ = 0;
			for(int i=0;i<sommets.size();i++){
				moyX+=sommets[i].at(0);
				moyY+=sommets[i].at(1);
				moyZ+=sommets[i].at(2);
			}
			Point<float,3> moy({moyX/sommets.size(),moyY/sommets.size(),moyZ/sommets.size()});

			float rayon = 0;
			for(int i=0;i<sommets.size();i++){
				Point<float,3> som3D = sommets[i].get3D();
				float dist = moy.length_to(som3D).norm();
				if(dist > rayon){
					rayon = dist;
				}
			}

			return Sphere<float>(moy,rayon);
		}

		// Retourne la n ième face de l'objet où n est passé en argument
		Triangle<float> face(unsigned int i)  {
			return mesh[i];
		}

		// Retourne le nombre de faces de l'objet
		unsigned int num_faces()  {
			return mesh.size();
		}

		// Ajoute une face à l'objet. Les 3 entiers en argument sont les sommets
		void add_face(unsigned int i, unsigned int j, unsigned int k){
			Point<float,3> somts[3] = { sommets[i].get3D(), sommets[j].get3D(), sommets[k].get3D() };
			mesh.push_back(Triangle<float>(somts));
		}

		// Ajoute un sommet à l'objet
		void add_sommet(Point<float,4> sommet){
			sommets.push_back(sommet);
		}

		// Supprime une face de l'objet. L'entier passé en arg se réfère à la liste de faces
		void remove_face(unsigned int i){
			mesh.erase(mesh.begin()+i);
		}
};

// Représente une scène
class Scene : public SceneInterface {
	private:
		Gui *gui;
		Camera &camera;
		vector<Object3D> objects;
	public:
		
		// Constructeur prenant en paramètre une caméra
		Scene(Camera &camera) : camera(camera) {
			gui = new Gui;
			gui->start();
		}

		// Détruit la scène
		~Scene(){
			gui->stop();
			delete gui;
		}

		// Retourne le nombre d'objets de la scène
		int nbObjects(){
			return objects.size();
		}

		// Pivote la caméra vers le haut
    virtual void press_up() override {
			camera.changingOrientationSpeed = 1;
			camera.orientation = (Quaternion<float>(1,Direction<float,3>(Vector<float,3>({0,0,1})))*camera.orientation).to_norm();
		}

		// Pivote la caméra vers le bas
    virtual void press_down() override {
			camera.orientation = (Quaternion<float>(1,Direction<float,3>(Vector<float,3>({0,0,-1})))*camera.orientation).to_norm();
		}

		// Pivote la caméra vers la gauche
    virtual void press_left() override {
			camera.changingOrientationSpeed = 1;
			camera.orientation = (Quaternion<float>(1,Direction<float,3>(Vector<float,3>({0,1,0})))*camera.orientation).to_norm();
		}

		// Pivote la caméra vers la droite
    virtual void press_right() override {
			camera.changingOrientationSpeed = 1;
			camera.orientation = (Quaternion<float>(1,Direction<float,3>(Vector<float,3>({0,-1,0})))*camera.orientation).to_norm();
		}


		// Fait un zoom in
    virtual void press_w() override {
			camera.zoomingSpeed = 1;
			camera.focalLenght+=0.01;
			camera.updateFocalDistance();
		}

		// Fait un zoom out
    virtual void press_x() override {
			if(camera.focalLenght-0.01 > 0){
				camera.zoomingSpeed = 1;
				camera.focalLenght-=0.01;
				camera.updateFocalDistance();
			}
		}

		// Déplace la caméra vers le bas
    virtual void press_s() override {
			camera.movingSpeed = 1;
			camera.position = Point<float,4>(Vector<float,4>({camera.position[0],camera.position[1]-0.1f,camera.position[2],camera.position[3]}));
			camera.update();
		}

		// Déplace la caméra vers la droite
    virtual void press_d() override {
			camera.movingSpeed = 1;
			camera.position = Point<float,4>(Vector<float,4>({camera.position[0]+0.1f,camera.position[1],camera.position[2],camera.position[3]}));
			camera.update();
		}

		// Déplace la caméra a gauche
    virtual void press_q() override {
			camera.movingSpeed = 1;
			camera.position = Point<float,4>(Vector<float,4>({camera.position[0]-0.1f,camera.position[1],camera.position[2],camera.position[3]}));
			camera.update();
		}

		// Déplace la caméra vers le haut
    virtual void press_z() override {
			camera.movingSpeed = 1;
			camera.position = Point<float,4>(Vector<float,4>({camera.position[0],camera.position[1]+0.1f,camera.position[2],camera.position[3]}));
			camera.update();
		}

		// Arrête la rotation de la caméra
    virtual void release_updown() override {
			camera.changingOrientationSpeed = 0;
		}

		// Arrête la rotation de la caméra
    virtual void release_leftright() override {
			camera.changingOrientationSpeed = 0;
		}

		// Arrête de zoomer in / out
    virtual void release_wx() override {
			camera.zoomingSpeed = 0;
		}

		// Arrête le déplacement de la caméra
    virtual void release_qd() override {
			camera.movingSpeed = 0;
		}

		// Arrête le déplacement de la caméra
    virtual void release_zs() override {
			camera.movingSpeed = 0;
		}

    virtual void update() override {
		}

		// Dessine tous les objets dans le champs de vision de la caméra
		virtual void draw() override  {
			//cout << "draww" << endl;
			for(int i=0;i<objects.size();i++){
				Sphere<float> bs = camera.get_transform().transform(objects[i].bsphere());

				/*Point<float,4> c= bs.getCenter().get4D();
				float r= bs.getRayon();
				Point<float,4> pt({c[0]+r,c[1]+r,c[2],1});
				Point<float,4> pt2({c[0]-r,c[1]-r,c[2],1});
				draw_edge(c,pt);
				draw_edge(c,pt2);	*/

				//if(!camera.outside_frustum(bs)){
					//cout << "dessine" << endl;
					draw_object(&objects[i]);
				//} else {
					//cout << "DESSINE PAS" << endl;
				//}
			}
			//cout << "draww" << endl;
		}

		// Dessine toutes les faces de l'objet passé en argument qui sont tournées vers la caméra.
		void draw_object( Object3D * obj)  {
			for(int i=0;i<obj->mesh.size();i++){
				//if(camera.sees(obj->mesh[i])){
					Triangle<float> tr = camera.get_transform().transform(obj->mesh[i]);
					draw_wire_triangle(tr);
				//}
			}
		}

		// Dessine la face passée en argument (les 3 arrêtes du triangle)
		void draw_wire_triangle(Triangle<float> &triangle)  {
				Point<float,4> pt1 = triangle.get_p0().get4D();
				Point<float,4> pt2 = triangle.get_p1().get4D();
				Point<float,4> pt3 = triangle.get_p2().get4D();
				//cout << "triangle" << endl;
				draw_edge(pt1,pt2);
				draw_edge(pt1,pt3);
				draw_edge(pt3,pt2);
				//cout << "triangle" << endl;
		}

		// Dessine le segment passé en argument
		void draw_edge( Point<float,4> &deb,  Point<float,4> &fin)  {	

			LineSegment<float,4> segment(deb,fin);

			try {
				//cout << deb << " - "<< fin << endl;
				LineSegment<float,4> visibleSeg = camera.visible_part(segment);
				Point<float,4> newDeb= visibleSeg.get_begin();
				Point<float,4> newFin= visibleSeg.get_end();

				//cout << "nv seg: " << newDeb << " - "<< newFin << endl;
				Point<float,4> debProjec = perspective_projection(newDeb);
				Point<float,4> finProjec = perspective_projection(newFin);
				//cout << "----> debProjec " << debProjec << endl;


				//cout << debProjec << " et " << finProjec << endl;
				Point<float,2> debNdc;
				Point<float,2> finNdc;

				if(debProjec[3] == 0.0f){
					debNdc = Point<float,2>(Vector<float,2>({debProjec[0],debProjec[1]}));
				} else {
					debNdc = Point<float,2>(Vector<float,2>({debProjec[0]/debProjec[3],debProjec[1]/debProjec[3]}));
				}

				if(finProjec[3] == 0.0f){
					finNdc = Point<float,2>(Vector<float,2>({finProjec[0],finProjec[1]}));
				} else {
					finNdc = Point<float,2>(Vector<float,2>({finProjec[0]/finProjec[3],finProjec[1]/finProjec[3]}));
				}

				
				gui->render_line(debNdc.getCoords(),finNdc.getCoords(), white);

			} catch(const out_of_range& e){
			}
		}

		

		// Projette le point passé en argument sur l'écran (near plane)
		Point<float,4> perspective_projection( Point<float,4> &point)  {
			
			float ratio = camera.height/camera.width;
			float b,t,r,l;
			b = (-ratio*0.1)/camera.focalLenght;
			t = (ratio*0.1)/camera.focalLenght;
			r = 0.1/camera.focalLenght;
			l = -0.1/camera.focalLenght;

			if(ratio > 1.0f){
				t *= ratio;
				b *=ratio;
			} else {
				r*=ratio;
				l*=ratio;
			}
			
			Plane<float> nTmp = camera.frustum->getPlane(0);
			Plane<float> fTmp = camera.frustum->getPlane(1);			

			Transformation transformation = Transformation(camera.position.dist(nTmp),camera.position.dist(fTmp),r,l,b,t);

			//Transformation finale = camera.get_transform().concat(transformation.getInverse());
			return transformation.getInverse().transform(point);
		}

		// Ajoute un objet à la scène
		void addObject(Object3D object){
			objects.push_back(object);
		}

		void launchLoop(){
			gui->main_loop(this);
		}

};

#endif
