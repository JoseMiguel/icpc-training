#include<iostream>
#include<list>
#include<set>
#include<sstream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<cstdio>
#include<cmath>
// teorema de pick area = adentro + puntos_en_los_lados(coor enteras)/2 - 1
using namespace std ;

const double PI = acos(-1.0);
const double EPS = 1e-9;
typedef long long Int;
int cmp( double a, double b = 0 ){
	return a + EPS < b ? -1 : a - EPS > b ? 1 : 0 ;
}

struct Point{
	double x, y;
	double ang;
	Point(){}
	Point( double x, double y ):x(x),y(y){}
	
	bool operator < ( const Point &p ) const {
		int t = cmp(x, p.x);
		if(t != 0) return t < 0;
		return cmp(y, p.y) < 0;
	}
	
	double operator %( Point p ) {
		return x * p.y - y * p.x;
	}
	double operator *( Point p ){
		return x * p.x + y * p.y;
	}

	Point operator *( double k ){
		return Point( x * k , y * k );
	}
	Point operator + ( Point p ){
		return Point( p.x + x , p.y + y );
	}
	Point operator - ( Point p ){
		return Point( x - p.x, y - p.y);
	}
	Point operator / ( double d ){
		return Point( x / d , y / d );
	}
	
};
typedef vector<Point> Poly;

int sign( double n ){
	return n < 0 ? -1 : 1;
}

double ccw( Point a, Point b, Point c ){
	return ( b - a ) % ( c - a );
}

double norma( Point a ) {
	return sqrt( a.x*a.x + a.y*a.y );
}

double dist( Point a, Point b ){
	return norma( b - a );
}

Point rot( Point p, double a ){
	return Point( -p.x*cos(a) + p.y * sin(a) ,  p.x * sin(a) + p.y*cos(a) );
}
double dist( Point p, Point a, Point b ){
	return fabs(ccw( p , a , b )) / norma( b - a );
}


int ccw2( Point a, Point b, Point c ){
	Point ba = b - a;
	Point ca = c - a;
	if ( cmp((ba % ca)) > 0 ) return 1;
	if ( cmp((ba % ca)) < 0 ) return -1;
	if ( cmp((ba * ca)) < 0 ) return 2;
	if ( cmp(norma(ba), norma(ca)) < 0 ) return -2;
	return 0;
}

bool exist_tri( double a, double b, double c ){
	return a + b > c && a + c > b && b + c > a;
}

//mira si el punto p esta dentro del qr
bool onSeg( Point p, Point q, Point r ){
	Point pq = q - p;
	Point pr = r - p;
	if ( cmp( pq % pr ) == 0 ) // revisa que sean colineales 
		if ( cmp( pq * pr ) <= 0 ) return true; // revisa que este al medio 
	return false;
}


bool intersect( Point a1, Point a2, Point b1, Point b2 ){

	int a =  ccw2( a1 , a2, b1 ) * ccw2( a1, a2, b2);
	int b =  ccw2( b1 , b2, a1 ) * ccw2( b1, b2, a2);
	if ( a > 0 || b > 0 ) return false;
	if ( a < 0 && b < 0 ) return true;
	return onSeg( a1, b1, b2 ) || onSeg( a2, b1, b2 ) || onSeg( b1, a1, a2 ) || onSeg( b2, a1, a2 ) ;
}


Point crossPoint( Point a1, Point a2, Point b1, Point b2 ) {
	// chekar si se intersecan antes
	double A = (a2-a1) % (b2-b1) ;
	double B = (a2-a1) % (a2-b1) ;

	double t = B / A;
	if ( fabs(A) < EPS && fabs(B) < EPS ) return b1;
	return b1 + (b2 - b1) * t; // P(t) = a + ( b-a ) * t
}

Int puntosEncima( Point A, Point B ){
	Int xx =  A.x - B.x, x, y, yy = A.y - B.y;
	if ( xx == 0 && yy == 0 ) return 0;
	Int mcd =__gcd(abs(xx),abs(yy));
	x = xx/ mcd , y = yy/ mcd;
	if ( xx ) return xx/x - 1;
	return yy/y - 1;
}


// sweep line

// para un conjunto de segmentos cuenta la cantidad de intersecciones en O(nlgn)
struct Event{
	int x, y, id ;
	bool f ;
	Event(){}
	Event( Point p, int id):id(id) {
		x = p.x;
		y = p.y;
	}
	bool operator < ( const Event &p)  const{
		int t = cmp( y , p.y );
		if ( t != 0 ) return t < 0 ;
		int s = cmp( x, p.x );
		if ( s != 0 ) return s < 0;
		if(f != p.f)return f;
		return id < p.id;
	}
};
// en este caso devuelve la cantidad de segmentos que no tienen cruces con nadie
int intersecciones( vector<pair<Point,Point> > seg ){
	int res = 0;
	set<Event> Q;
	vector< int > cross( seg.size() , 0);		
	vector< int > sweep;
	for( int i = 0; i < seg.size(); i++){
		Event q( seg[i].first, i);
		Event r( seg[i].second, i);
		if ( r < q ) swap( q, r );
		q.f = true; r.f = false; 
		Q.insert(q); 
		Q.insert(r);
	}
	while( Q.size() > 0 ){
		Event p = *Q.begin(); 
		Q.erase( Q.begin() );
		if ( p.f ){
			for( int i = 0; i < sweep.size(); i++){
				bool b = intersect( seg[ sweep[i] ].first, seg[ sweep[i] ].second, seg[ p.id ].first, seg[ p.id ].second);
				if ( b ) {
					cross[ p.id ]++;
					cross[ sweep[i] ]++;
				}
			}
			sweep.push_back( p.id ); 
		}else{
			vector<int>::iterator it = find(sweep.begin(),sweep.end(), p.id );
			if ( it != sweep.end() ) sweep.erase( it );
		}
	}
	for( int i= 0; i < cross.size(); i++){
		if ( cross[i] == 0 ) res++;
	}
	return res;
}


//pendiente
double slope( Point a, Point b ){
	if ( a.x == b.x ) return 1e7;
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return dy / dx;
}
// alineacion maxima de puntos en base a su pendiente N^2lgn

bool sort_y( Point a, Point b){
	if ( cmp(a.y,b.y) == 0 )return cmp( a.x , b.x ) < 0;
	return cmp( a.y , b.y ) < 0;
}
double pd[ 1005 ];
int lining( Poly p ){
	int n = p.size(), res = 2;
	if ( n <= 1 ) return n;

	sort( p.begin(), p.end(), sort_y);
	for( int i = 0; i < n; i++){
		int k = 0;
		for( int j = i + 1 ; j < n; j++ ){
			pd[ k++ ] = slope( p[j], p[i] );
		}
		sort( pd, pd + k );
		int tmp = 2;
		for( int j = 1; j < k; j++){
			if ( cmp(fabs( pd[j] - pd[j-1] )) == 0 ) tmp++;
			else tmp = 2;
			res = max( tmp , res );
		}
	}
	return res ;
}

// Ax + By = C
struct Recta{
	double A, B, C;
	Recta(){}
	// despeja en base al segment ab
	Recta( Point a, Point b ){
		A = b.y - a.y;
		B = a.x - b.x;
		C = A*a.x + B*a.y;
	}
	Point intersect( Recta r, bool &paralelas ) {
		double det = A*r.B - r.A*B;
		if ( det == 0 ) {
			paralelas = true;
			return Point(1<<20,1<<20);
		}else{
			double x = (r.B*C - B*r.C) / det;
			double y = (A*r.C - r.A*C) / det;
			return Point( x, y );
		}
	}
	Point refleja( Point p ) {
		Recta r;
		r.A = -B;
		r.B = A;
		r.C = -B*p.x + A*p.y;
		bool paralelas = false;
		Point inter =  intersect( r, paralelas);
		return inter - ( p - inter );
	}
};


//Poligonos
double area( Poly p ){
	double res = 0.0;
	int n = p.size();
	for( int i = 0; i < n; i++){
		res += p[ i ] % p[ ( i + 1 ) % n ];
	}
	return fabs(res / 2.0 );
}

Point centerMass( Poly &p ){
	Point c( 0 , 0 ) ;
	for( int i = 0; i < p.size(); i++ ) {
		c.x += p[i].x ;/// (double)p.size();
		c.y += p[i].y ;/// (double)p.size();
	}
	return c / (double) p.size();
}

// convex hull 

Poly convexHull( Poly p ){
	sort( p.begin(), p.end() );
	int n = p.size(), k = 0;
	Poly h ( 2 * n );
	for( int i = 0; i < n; i++ ){
		while( k >= 2 && ccw( h[k-2], h[k-1], p[i] ) <= 0 ) k--;
		h[k++] = p[i];
	}
	int t = k + 1;
	for( int i = n - 2; i >= 0; i-- ){
		while( k >= t && ccw( h[k-2], h[k-1], p[i] ) <= 0 ) k--;
		h[k++] = p[i];
	}
	// devuelve el primero y el ultimo punto iguales, por eso se le resta 1
	h.resize( k );
	return h;
	
}

// punto dentro de un triangualo
// debe estar en sentio antihorario 
bool inTriang( const Poly &T , const Point &p){
	return ccw2( T[0], T[1], p ) >= 0 && 
		ccw2( T[1],T[2],p ) >= 0 && 
		ccw2( T[2],T[0],p) >= 0; 
}
bool inTriang2( const Poly &T , const Point &p){
	return ccw2( T[0], T[1], p ) <= 0 && 
		ccw2( T[1],T[2],p ) <= 0 && 
		ccw2( T[2],T[0],p) <= 0; 
}
// triangular un poligono en O(2*n^2) devuelve un vector de triangulos
bool can( const Poly &t, const  Poly &p, int i , int j , int k ){
	if( cmp( ccw(t[0], t[1], t[2])) <= 0) return false; // si esta tipo para afuera cago ps
	for(int l = 0; l < p.size(); l++){ // revisa que ningun punto este dentro del triangulo que formamos
		if(l != i && l != j && l != k)
			if( inTriang(t, p[l]) || inTriang2( t, p[l]))	return false;
	}
	return true;
}
// debe ser antihorario
vector<Poly> triangulate( const Poly &p ){
	vector<Poly> res;
	int n = p.size();
	vector<int> l, r;
	for( int i = 0; i < n; i++){
		l.push_back( ( i - 1 + n) % n );
		r.push_back( ( i + 1) % n ); // crea una lista doblemente enlazada
	}
	int i = n - 1, cagao = 0;
	while( res.size() < n - 2 ){
		if ( cagao >= n ) return vector<Poly>();
		i = r[i]; // avanza tipo un i++
		Poly tmp; 
		tmp.push_back( p[l[i]] );
		tmp.push_back( p[i] );
		tmp.push_back( p[r[i]] ); // crea un triangulo

		if ( can( tmp, p , l[i], i , r[i] ) ){ // checa si sirve
			res.push_back( tmp ); // guardamos la solucion
			l[ r[i] ] = l[i];
			r[ l[i] ] = r[i]; // con estas dos operaciones en O(1) borramos el punto del "medio" del triangulo
			cagao = 0; // no fallo
		}else cagao++; // se fue al carajo
	}
	return res;
}

// si un punto esta dentro de un poligono

bool inPoly( Poly &P, Point p ){
	int cross = 0, n = P.size();
	for( int i = 1; i <= n; i++){
		Point q = P[i-1]; 
		Point r = P[i%n]; 

		// qr es un lado del poligono
		if ( q.y > r.y ) swap( q, r ); // por comodidad hacer que r siempre este arriba de q
		double ori = ccw( p ,q , r ); // la orientacion 
		if ( p.y > q.y && p.y <= r.y ){ // esta tipo al medio de un segmento... el intervalo debe ser asi... (q, r] 
		// de tal forma que en la siguiente iteracion no se repita el r
			if ( ori > 0 ) { // si el segmento esta tipo a la derecha 
				cross++;
			}
			else if ( ori == 0 ) return true;
		}
	}
	return cross % 2;

}
// sentido antihorario
bool inConvexPoly( Poly &p , Point k ){
	int ori = ccw2( p[0], p[1], p[2] );

	int n = p.size(), lo = 1, hi = n - 1;
	while( lo + 1 < hi ){
		int mid = ( lo + hi ) / 2;
		if ( sign(ccw2( p[0], p[mid], k )) == sign(ori) ) lo = mid;
		else hi = mid;
	}
	Poly tri( 3 ); 
	tri[ 0 ] = p[ 0 ]; tri[ 1 ] = p[ lo ]; tri[ 2 ] = p[ hi ];
	return inTriang( tri , k );

}
// arma un poligono simple tipo random con varios puntos
void simplePolygon( Poly &p ){
	Point c = centerMass( p ) ;
	for( int i = 0; i < p.size(); i++){
		p[i].ang = atan2( c.x - p[i].x , c.y - p[i].y );
	}
	sort( p.begin(), p.end() );
}


int main(){

	int runs, r = 1, n;
	scanf("%d",&runs );
	while( runs-- ){
		scanf("%d",&n);
		Point c( 0 ,0 );
		Poly p( n );
		for( int i = 0; i < n; i++ ) {
			int x, y;
			scanf("%d %d", &x, &y );
			c.x += x / (double ) n , c.y += y /( double) n ;
			p[i] = Point( x, y );
		}
		if ( n == 1 ) {
			printf("Case #%d: 0.000\n", r++);
			continue;
		}

		Poly h = convexHull( p );
		double res = 1e30;
		for( int i = 0; i < h.size() - 1; i++ ){
			Point q = h[ i ], r = h[ i + 1 ];
			res = min( res, dist( c, q , r ) );
		}
		printf("Case #%d: %.3lf\n",r++ , res );

	}

	return 0;
}


