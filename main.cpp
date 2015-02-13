#include <iostream>
#include <string>

#include "ss_util/ss.h"
#include "ss_util/checksum.h"
#include "ss_util/system_process.h"
#include "ss_util/timestamp.h"

#include "ss_util/math/vector.h"
#include "ss_util/math/matrix.h"
#include "ss_util/math/bbox.h"

#include "ss_util/structures/fixed_array.h"
#include "ss_util/structures/static_pool.h"

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
bool test_string( const char * str1, const char * str2, bool insensitive = true )
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
void test_bbox_aligned( vector2 &c1, float r1, vector2 &c2, float r2 )
{
	bbox_aligned box1(c1 - vector2(r1,r1), c1 + vector2(r1,r1));
	bbox_aligned box2(c2 - vector2(r2,r2), c2 + vector2(r2,r2));
	
	if ( box1.collides(box2) ) {
		cout << "Boxes Collide" << endl;
	} else {
		cout << "Boxes Miss" << endl;
	}
}

struct string_container
{
	static const int size = 32;
	string_container *prev, *next;
	char val[size];
	
	string_container()
	{
		val[0] = '\0';
	}
	
	string_container(const char *str)
	{
		prev = next = NULL;
		set(str);
	}
	
	void set( const char *str )
	{
		memcpy(val, str, size);
	}
};

// Example of fixed array.
//
void print_fixed_array( fixed_array<string_container> *array )
{
	cout << "Printing Fixed Array - " << array->size() << "/" << array->max_size() << " used" << endl;

	for ( int i = 0; i < array->size(); i++ )
	{
		cout << array->pointer_at(i)->val << endl;	
	}
}

void test_fixed_array()
{
	const int FIXED_ARRAY_SIZE = 8;
	fixed_array<string_container> my_array(FIXED_ARRAY_SIZE);
	my_array.append(string_container("Frankie"));
	my_array.append(string_container("Joey"));
	my_array.insert_at_index(string_container("Ronaldo"), 1);
	
	print_fixed_array(&my_array);
	
	my_array.remove_at_index(1);
	
	print_fixed_array(&my_array);
}

// Example of static pool.
//
void print_string_pool( static_pool<string_container> *pool )
{
	cout << "Printing Static Pool - " << pool->num_items - pool->num_free << "/" << pool->num_items << " used" << endl;
	string_container *cur = NULL;
	DL_FOREACH(pool->used_lists[0], cur)
	{
		cout << cur->val << endl;
	}
}

void test_static_pool()
{
	const int NUM_STRINGS = 8;
	static_pool<string_container> *test_pool = new static_pool<string_container>( NUM_STRINGS );
	string_container *string1 = test_pool->alloc();
	string_container *string2 = test_pool->alloc();
	string_container *string3 = test_pool->alloc();
	string_container *string4 = test_pool->alloc();
	
	// Populate the values allocated from the pool.
	string1->set("Spongebob");
	string2->set("Ren");
	string3->set("Spongebob");
	string4->set("Stimpy");
	
	print_string_pool(test_pool);
	
	// Remove all values "Spongebob"
	string_container *cur = NULL, *next = NULL;
	DL_FOREACH_DELETE_SAFE(test_pool->used_lists[0], cur, next)
	{
		if ( strcmp(cur->val, "Spongebob") == 0)
		{
			test_pool->free(cur);
		}
	}
	
	print_string_pool(test_pool);
	
	delete test_pool;
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
	
	// Test fixed array.
	//
	test_fixed_array();
	
	// Test static pools.
	//
	test_static_pool();
	
	
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