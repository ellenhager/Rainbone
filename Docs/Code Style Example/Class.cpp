#include "class.h" // in the cpp file we only include the header to our class, nothing more, nothing less

// Constructor with some arguments
Class::Class(int n, float f, std::string s)
: mNum(n), mFloat(f)m mString(s) {  // Set instance variables that we pass to the constructor like this

    // Do other constructor stuff
}

// Destructor
Class::~Class() {

    // Clear all containers and deallocate memory
}

// Example function, name the input variables so that the name explains what they accualy are.
Class::publicFunction(int n, float f) {

    // Standard for-loop
    for(unsigned int i = 0; i < mFloats.size(); i++) {

        if(i < n) {
            // do something
        }
    }

    // A short for-loop with only one line of executions can be written like this
    for(unsigned int i = 0; i < n; i++)
        // do somehing


    // A vector with pointer objects is traversed with an iterator like this
    for(std::vector<Object *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it) {
        // Do stuff here, for example call a function
        (*it)->someFunction();
    }

    // Long lines are divided like this
    scene_mat = glm::rotate( 
        scene_mat,                                          // Comment each argument if required
        static_cast<float>( curr_time.getVal() * speed ),   // Comment each argument if required
        glm::vec3(0.0f, -1.0f, 0.0f)                        // Comment each argument if required
    );

}