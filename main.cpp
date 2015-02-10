#include <iostream>
#include <string>

#include "ss_util/ss.h"
#include "ss_util/checksum.h"
#include "ss_util/system_process.h"
#include "ss_util/timestamp.h"

#include "ss_util/math/vector.h"
#include "ss_util/math/matrix.h"
#include "ss_util/math/bbox.h"

using namespace std;

// Example of using system_process
//
class test_process : public system_process {
private:
	timestamp click_timer;
	int click_counter;
	static const int TICK_MS = 1000;
public:
	
	test_process() {
		click_counter = 5;
		click_timer.set(TICK_MS);
	}
	
	void process() {
		system_process::process();
		
		if ( click_timer.elapsed() ) {
			cout << "T-Minus: " << click_counter << endl;
			click_counter--;
			click_timer.set(TICK_MS);
		}
	}
	
	bool is_finished()
	{
		return click_counter <= 0;
	}
};

// Example of using checksum
//
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


// Example of using matrices/vectors.
//
void print_vector( const vector2 &v )
{
	cout << "[\t" << v.x << "\t" << v.y << "\t]" << endl;
}

void print_matrix( const matrix &m )
{
	print_vector(m.rvec);
	print_vector(m.uvec);
}


// Example of using bounding boxes.
//
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

// Example of using system_process.
//
void test_system_process( float timescale = 1.0f )
{
	// Set timescale.
	ss_set_timescale(timescale);
	cout << "Speed: " << fl2i(timescale * 100.0f) << "%" << endl;
	
	// Create a test process.
	test_process *proc = new test_process();
	
	// Do frame as long as no quit message is posted.
	//
	while ( !proc->is_finished() ) {
		ss_do_frame();
	}
	
	// Kill the process.
	delete proc;
	proc = NULL;
	
	// Return timescale to normal.
	ss_set_timescale(1.0f);
}

int main() {
	
	// Test checksum on strings.
	//
	test_string( "Mikey", "Mikey" );
	test_string( "Mikey", "mikey" );
	test_string( "Mikey", "mikey", false );
	
	// Test vectors/matrices.
	//
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
	
	// Test system processes.
	//
	ss_initialize();
	
	// Show examples of speed.
	test_system_process();
	test_system_process(2.0f);
	test_system_process(0.5f);
	
	ss_shutdown();
	
	return 0;
}