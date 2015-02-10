#include <iostream>
#include <string>

#include "ss_util/checksum.h"

#include "ss_util/math/vector.h"
#include "ss_util/math/matrix.h"

#include "ss_util/math/bbox.h"

using namespace std;

bool test_string(const char * str1, const char * str2, bool insensitive = true)
{
	bool equal = false;
	
	if ( insensitive ) {
		equal = checksum_stri(str1) == checksum_stri(str2);
		cout << "CASE-INSENSITIVE: ";
	} else {
		equal = checksum(str1) == checksum(str2);
		cout << "CASE-SENSITIVE: ";
	}
	
	if ( equal ) {
		cout << str1 << " is equal to " << str2 << endl;
	} else {
		cout << str1 << " is NOT equal to " << str2 << endl;
	}
	
	return equal;
}

void print_vector( const vector2 &v )
{
	cout << "[\t" << v.x << "\t" << v.y << "\t]" << endl;
}

void print_matrix( const matrix &m )
{
	print_vector(m.rvec);
	print_vector(m.uvec);
}

void test_bbox_aligned(vector2 &c1, float r1, vector2 &c2, float r2)
{
	bbox_aligned box1(c1 - vector2(r1,r1), c1 + vector2(r1,r1));
	bbox_aligned box2(c2 - vector2(r2,r2), c2 + vector2(r2,r2));
	
	if ( box1.collides(box2) ) {
		cout << "Boxes Collide" << endl;
	} else {
		cout << "Boxes Miss" << endl;
	}
}

int main() {
	
	// Test checksum on strings.
	test_string( "Mikey", "Mikey" );
	test_string( "Mikey", "mikey" );
	test_string( "Mikey", "mikey", false );
	
	// Test vectors/matrices.
	vector2 up_vector(0.0f, 1.0f);

	cout << "Up Vector: ";
	print_vector(up_vector);
	
	matrix id_matrix;
	id_matrix.make_uvec(up_vector);

	cout << "\nIdentity Matrix" << endl;
	print_matrix(id_matrix);
	
	matrix rotated = id_matrix;
	rotated.rotate(3.14159f * 0.25f);
	cout << "\nRotated 45 Degrees" << endl;
	print_matrix(rotated);
	
	cout << "\nTesting Bounding Boxes" << endl;
	vector2 origin(0.0f,0.0f);
	vector2 two_five(2.5f,2.5f);
	test_bbox_aligned(origin, 5.0f, two_five, 5.0f);		// Collide
	test_bbox_aligned(origin, 1.0f, two_five, 1.0f);		// Miss
	
	return 0;
}