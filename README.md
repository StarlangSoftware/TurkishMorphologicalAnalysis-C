Morphological Analysis
============

## Morphology

In linguistics, the term morphology refers to the study of the internal structure of words. Each word is assumed to consist of one or more morphemes, which can be defined as the smallest linguistic unit having a particular meaning or grammatical function. One can come across morphologically simplex words, i.e. roots, as well as morphologically complex ones, such as compounds or affixed forms.

Batı-lı-laş-tır-ıl-ama-yan-lar-dan-mış-ız 
west-With-Make-Caus-Pass-Neg.Abil-Nom-Pl-Abl-Evid-A3Pl
‘It appears that we are among the ones that cannot be westernized.’

The morphemes that constitute a word combine in a (more or less) strict order. Most morphologically complex words are in the ”ROOT-SUFFIX1-SUFFIX2-...” structure. Affixes have two types: (i) derivational affixes, which change the meaning and sometimes also the grammatical category of the base they are attached to, and (ii) inflectional affixes serving particular grammatical functions. In general, derivational suffixes precede inflectional ones. The order of derivational suffixes is reflected on the meaning of the derived form. For instance, consider the combination of the noun göz ‘eye’ with two derivational suffixes -lIK and -CI: Even though the same three morphemes are used, the meaning of a word like gözcülük ‘scouting’ is clearly different from that of gözlükçü ‘optician’.

## Dilbaz

Here we present a new morphological analyzer, which is (i) open: The latest version of source codes, the lexicon, and the morphotactic rule engine are all available here, (ii) extendible: One of the disadvantages of other morphological analyzers is that their lexicons are fixed or unmodifiable, which prevents to add new bare-forms to the morphological analyzer. In our morphological analyzer, the lexicon is in text form and is easily modifiable, (iii) fast: Morphological analysis is one of the core components of any NLP process. It must be very fast to handle huge corpora. Compared to other morphological analyzers, our analyzer is capable of analyzing hundreds of thousands words per second, which makes it one of the fastest Turkish morphological analyzers available.

The morphological analyzer consists of five main components, namely, a lexicon, a finite state transducer, a rule engine for suffixation, a trie data structure, and a least recently used (LRU) cache.

In this analyzer, we assume all idiosyncratic information to be encoded in the lexicon. While phonologically conditioned allomorphy will be dealt with by the transducer, other types of allomorphy, all exceptional forms to otherwise regular processes, as well as words formed through derivation (except for the few transparently compositional derivational suffixes are considered to be included in the lexicon.

In our morphological analyzer, finite state transducer is encoded in an xml file.

To overcome the irregularities and also to accelerate the search for the bareforms, we use a trie data structure in our morphological analyzer, and store all words in our lexicon in that data structure. For the regular words, we only store that word in our trie, whereas for irregular words we store both the original form and some prefix of that word. 

Simple Web Interface
============
[Link 1](http://104.247.163.162/nlptoolkit/turkish-morphological-analysis.html) [Link 2](https://starlangsoftware.github.io/nlptoolkit-web-simple/turkish-morphological-analysis.html)

Video Lectures
============

[<img src="https://github.com/StarlangSoftware/TurkishMorphologicalAnalysis/blob/master/video1.jpg" width="50%">](https://youtu.be/KxguxpbgDQc)[<img src="https://github.com/StarlangSoftware/TurkishMorphologicalAnalysis/blob/master/video2.jpg" width="50%">](https://youtu.be/UMmA2LMkAkw)[<img src="https://github.com/StarlangSoftware/TurkishMorphologicalAnalysis/blob/master/video3.jpg" width="50%">](https://youtu.be/dP97ovMSSfE)[<img src="https://github.com/StarlangSoftware/TurkishMorphologicalAnalysis/blob/master/video4.jpg" width="50%">](https://youtu.be/Tgmy5tts_pY)

For Developers
============

You can also see [Java](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis), [Python](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-Py), [Cython](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-Cy), [Swift](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-Swift), [Js](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-Js), [Php](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-Php), [C++](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-CPP), or [C#](https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-CS) repository.

## Requirements

* [C Compiler](#cpp)
* [Git](#git)


### CPP
To check if you have compatible C Compiler installed,
* Open CLion IDE 
* Preferences >Build,Execution,Deployment > Toolchain  

### Git

Install the [latest version of Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).

## Download Code

In order to work on code, create a fork from GitHub page. 
Use Git for cloning the code to your local or below line for Ubuntu:

	git clone <your-fork-git-link>

A directory called TurkishMorphologicalAnalysis-CPP will be created. Or you can use below link for exploring the code:

	git clone https://github.com/starlangsoftware/TurkishMorphologicalAnalysis-C.git

## Open project with CLion IDE

To import projects from Git with version control:

* Open CLion IDE , select Get From Version Control.

* In the Import window, click URL tab and paste github URL.

* Click open as Project.

Result: The imported project is listed in the Project Explorer view and files are loaded.


## Compile

**From IDE**

After being done with the downloading and opening project, select **Build Project** option from **Build** menu. After compilation process, user can run TurkishMorphologicalAnalysis-C.

Detailed Description
============

+ [Creating FsmMorphologicalAnalyzer](#creating-fsmmorphologicalanalyzer)
+ [Word level morphological analysis](#word-level-morphological-analysis)
+ [Sentence level morphological analysis](#sentence-level-morphological-analysis)

## Creating FsmMorphologicalAnalyzer 

FsmMorphologicalAnalyzer provides Turkish morphological analysis. This class can be created as follows:

    FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer();
    
This generates a new `TxtDictionary` type dictionary from [`turkish_dictionary.txt`](https://github.com/olcaytaner/Dictionary/tree/master/src/main/resources) with fixed cache size 100000 and by using [`turkish_finite_state_machine.xml`](https://github.com/olcaytaner/MorphologicalAnalysis/tree/master/src/main/resources). 

Creating a morphological analyzer with different cache size, dictionary or finite state machine is also possible. 
* With different cache size, 

        FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer(50000);   

* Using a different dictionary,

        FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer("my_turkish_dictionary.txt");   

* Specifying both finite state machine and dictionary, 

        FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer("fsm.xml", "my_turkish_dictionary.txt") ;      
    
* Giving finite state machine and cache size with creating `TxtDictionary` object, 
        
        TxtDictionary dictionary = new TxtDictionary("my_turkish_dictionary.txt", new TurkishWordComparator());
        FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer("fsm.xml", dictionary, 50000) ;
    
* With different finite state machine and creating `TxtDictionary` object,
       
        TxtDictionary dictionary = new TxtDictionary("my_turkish_dictionary.txt", new TurkishWordComparator(), "my_turkish_misspelled.txt");
        FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer("fsm.xml", dictionary);

## Word level morphological analysis

For morphological analysis,  `morphologicalAnalysis(String word)` method of `FsmMorphologicalAnalyzer` is used. This returns `FsmParseList` object. 


    FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer();
    String word = "yarına";
    FsmParseList fsmParseList = fsm.morphologicalAnalysis(word);
    for (int i = 0; i < fsmParseList.size(); i++){
      System.out.println(fsmParseList.getFsmParse(i).transitionList();
    } 
      
Output

    yar+NOUN+A3SG+P2SG+DAT
    yar+NOUN+A3SG+P3SG+DAT
    yarı+NOUN+A3SG+P2SG+DAT
    yarın+NOUN+A3SG+PNON+DAT
    
From `FsmParseList`, a single `FsmParse` can be obtained as follows:

    FsmParse parse = fsmParseList.getFsmParse(0);
    System.out.println(parse.transitionList();   
    
Output    
    
    yar+NOUN+A3SG+P2SG+DAT
    
## Sentence level morphological analysis
`morphologicalAnalysis(Sentence sentence)` method of `FsmMorphologicalAnalyzer` is used. This returns `FsmParseList[]` object. 

    FsmMorphologicalAnalyzer fsm = new FsmMorphologicalAnalyzer();
    Sentence sentence = new Sentence("Yarın doktora gidecekler");
    FsmParseList[] parseLists = fsm.morphologicalAnalysis(sentence);
    for(int i = 0; i < parseLists.length; i++){
        for(int j = 0; j < parseLists[i].size(); j++){
            FsmParse parse = parseLists[i].getFsmParse(j);
            System.out.println(parse.transitionList());
        }
        System.out.println("-----------------");
    }
    
Output
    
    -----------------
    yar+NOUN+A3SG+P2SG+NOM
    yar+NOUN+A3SG+PNON+GEN
    yar+VERB+POS+IMP+A2PL
    yarı+NOUN+A3SG+P2SG+NOM
    yarın+NOUN+A3SG+PNON+NOM
    -----------------
    doktor+NOUN+A3SG+PNON+DAT
    doktora+NOUN+A3SG+PNON+NOM
    -----------------
    git+VERB+POS+FUT+A3PL
    git+VERB+POS^DB+NOUN+FUTPART+A3PL+PNON+NOM

# Cite

	@inproceedings{yildiz-etal-2019-open,
    	title = "An Open, Extendible, and Fast {T}urkish Morphological Analyzer",
    	author = {Y{\i}ld{\i}z, Olcay Taner  and
      	Avar, Beg{\"u}m  and
      	Ercan, G{\"o}khan},
    	booktitle = "Proceedings of the International Conference on Recent Advances in Natural Language Processing (RANLP 2019)",
    	month = sep,
    	year = "2019",
    	address = "Varna, Bulgaria",
    	publisher = "INCOMA Ltd.",
    	url = "https://www.aclweb.org/anthology/R19-1156",
    	doi = "10.26615/978-954-452-056-4_156",
    	pages = "1364--1372",
	}

For Contibutors
============

### Conan Setup

1. First install conan.

pip install conan

Instructions are given in the following page:

https://docs.conan.io/2/installation.html

2. Add conan remote 'ozyegin' with IP: 104.247.163.162 with the following command:

conan remote add ozyegin http://104.247.163.162:8081/artifactory/api/conan/conan-local --insert

3. Use the comman conan list to check for installed packages. Probably there are no installed packages.

conan list

### conanfile.py file

1. Put the correct dependencies in the requires part
```
    requires = ["math_c/1.0.0", "data_structure_c/1.0.0", "classification_c/1.0.0"]
```

2. Default settings are:
```
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}
    exports_sources = "src/*", "Test/*"

    def layout(self):
        cmake_layout(self, src_folder="src")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(conanfile=self, keep_path=False, src=join(self.source_folder), dst=join(self.package_folder, "include"), pattern="*.h")
        copy(conanfile=self, keep_path=False, src=self.build_folder, dst=join(self.package_folder, "lib"), pattern="*.a")
        copy(conanfile=self, keep_path=False, src=self.build_folder, dst=join(self.package_folder, "lib"), pattern="*.so")
        copy(conanfile=self, keep_path=False, src=self.build_folder, dst=join(self.package_folder, "lib"), pattern="*.dylib")
        copy(conanfile=self, keep_path=False, src=self.build_folder, dst=join(self.package_folder, "bin"), pattern="*.dll")

    def package_info(self):
        self.cpp_info.libs = ["ComputationalGraph"]
```

### CMakeLists.txt file
1. Set the C standard with compiler flags.
```
	set(CMAKE_C_STANDARD 17)
	set(CMAKE_C_FLAGS "-O3")
```
2. Dependent packages should be given with find_package.
```
	find_package(util_c REQUIRED)
	find_package(data_structure_c REQUIRED)
```
3. For library part, use add_library and target_link_libraries commands. Use m library for math linker in Linux.
```
	add_library(Math src/Distribution.c src/Distribution.h src/DiscreteDistribution.c src/DiscreteDistribution.h src/Vector.c src/Vector.h src/Eigenvector.c src/Eigenvector.h src/Matrix.c src/Matrix.h src/Tensor.c src/Tensor.h)
	target_link_libraries(Math util_c::util_c data_structure_c::data_structure_c m)
```
4. For executable tests, use add_executable and target_link_libraries commands. Use m library for math linker in Linux.
```
	add_executable(DiscreteDistributionTest src/Distribution.c src/Distribution.h src/DiscreteDistribution.c src/DiscreteDistribution.h src/Vector.c src/Vector.h src/Eigenvector.c src/Eigenvector.h src/Matrix.c src/Matrix.h src/Tensor.c src/Tensor.h Test/DiscreteDistributionTest.c)
	target_link_libraries(DiscreteDistributionTest util_c::util_c data_structure_c::data_structure_c m)
```

### Data files
1. Add data files to the cmake-build-debug folder.

### C files
1. Define structs as class counterparts.
```
	typedef struct {
	    double *data;       // Flattened data array
 	   	int *shape;         // Shape of the tensor
	    int *strides;       // Strides for each dimension
 	   	int dimensions;     // Number of dimensions
  	  	int total_elements;
	} Tensor;
```
2. Define also typedefs for pointer to those structs.
```
	typedef Tensor *Tensor_ptr;
```
3. Put all those definitions to the header files with prototypes of functions.
4. Constructor method(s) should allocate a pointer of struct and fill the details and return the pointer.
```
Vector_ptr create_vector(Array_list_ptr values) {
    Vector_ptr result = malloc_(sizeof(Vector));
    result->values = values;
    result->size = values->size;
    return result;
}
```
5. Destructor method should be defined for each struct and deallocate the object and its contents if allocated.
```
void free_vector(Vector_ptr vector) {
    free_array_list(vector->values, free_);
    free_(vector);
}
```
6. If there is multiple inheritance, then define the super class inside the subclass, which also should be defined inside its own subclass etc.
```
struct sgd_momentum {
    Optimizer optimizer;
    Hash_map_ptr velocity_map;
    double momentum;
};
struct Adam {
    Sgd_momentum sgd;
    Hash_map_ptr momentum_map;
    double beta2;
    double epsilon;
    double current_beta1;
    double current_beta2;
};
struct AdamW {
    Adam adam;
    double weight_decay;
};
```
7. If there is overloading, and subclasses should override the method, you can add pointers to the functions and set them in the constructor.
```
struct function {
    Function_type function_type;
    Tensor_ptr (*calculate)(const void*, const Tensor*);
    Tensor_ptr (*derivative)(const void*, const Tensor*, const Tensor*);
};
Elu_ptr create_elu(const double a) {
    Elu_ptr delu = malloc_(sizeof(Elu));
    delu->a = a;
    delu->function.function_type = ELU;
    delu->function.calculate = calculate_elu;
    delu->function.derivative = derivative_elu;
    return delu;
}
```
8. Instanceof could be done using an enumerated type defined in the parent class.
```
enum optimizer_type {
    OPTIMIZER, ADAM, ADAM_W, SGD_MOMENTUM, SGD,
};
typedef enum optimizer_type Optimizer_type;
struct optimizer {
    Optimizer_type type;
```
9. If needed, hash function should be declared for the struct.
```
	unsigned int hash_function_computational_node(const Computational_node *node, int N) {
    	return ((uintptr_t)node) % N;
	}
```
10. If needed, comparator function should be declared for the struct.
```
int compare_annotated_word(const Annotated_word *word1, const Annotated_word *word2) {
    return compare_string(word1->name, word2->name);
}
```
11. Data Structures: For hashmap, linked hash map and hashset, use create_hash_map, create_linked_hash_map, and create_hash_set in data structure library. There are default string and integer versions of hash_map.
```
Hash_map_ptr create_hash_map(unsigned int (*hash_function)(const void *, int), int (*key_compare)(const void *, const void *));
Linked_hash_map_ptr create_linked_hash_map(unsigned int (*hash_function)(const void *, int), int (*compare)(const void *, const void *));
Hash_map_ptr create_string_hash_map();
Hash_map_ptr create_integer_hash_map();
Hash_set_ptr create_hash_set(unsigned int (*hash_function)(const void *, int), int (*compare)(const void *, const void *));
```
12. Data Structures: For array list, use create_array_list in data structure library..
```
Array_list_ptr create_array_list();
```
13. Data Structures: For linked list, use linked_list in data structure library..
```
Linked_list_ptr create_linked_list(int (*compare)(const void *, const void *));
```
14. Data Structures: For queue and stack, use queue and stack in data structure library.
```
Queue_ptr create_queue();
Stack_ptr create_stack();
```
15. Data Structures: For hash map that is counting items, use counter hash map in data structure library.
```
Counter_hash_map_ptr
create_counter_hash_map(unsigned int (*hash_function)(const void *, int), int (*key_compare)(const void *, const void *));
```
16. Do not forget to comment each function.
```
	/**
 	* Initializes a tensor with given data and shape. Data is cloned in the tensor
	*
 	* @param data Flattened array representing the tensor data.
 	* @param shape Array representing the shape of the tensor.
 	* @param dimensions Size of the shape array.
 	* @return Pointer to the created tensor. Returns NULL on failure.
 	*/
	Tensor_ptr create_tensor(const double *data, const int *shape, int dimensions) {
```
17. Function names should follow snake case case.
```
	Tensor_ptr tensor_get(const Tensor *tensor, const int *dimensions, int size) {
```
18. All heap allocations should be done with malloc_, calloc_, realloc_ taken from the memory management library.
```
    int *new_shape = malloc_((tensor->dimensions - size) * sizeof(int));
```
19. All heap deallocations should be done with free_ taken from the memory management library.
```
    free_(new_shape);
```
20. DO NOT write getter and setter methods.
21. If there are multiple constructors for a class, define them as constructor1, constructor2, ....
```
Tensor_ptr create_tensor(const double *data, const int *shape, int dimensions);

Tensor_ptr create_tensor2(const int *shape, int dimensions);

Tensor_ptr create_tensor3(double *data, const int *shape, int dimensions);

Tensor_ptr create_tensor4(const int *shape, int dimensions);
```
22. Use separate main method for testing purposes.
```
int main() {
    start_medium_memory_check();
    testColumnWiseNormalize();
    testMultiplyWithConstant();
    testDivideByConstant();
    end_memory_check();
}
```
23. Testing main method should start with memory check and end with memory check.
```
int main() {
    start_medium_memory_check();
    testColumnWiseNormalize();
    testMultiplyWithConstant();
    testDivideByConstant();
    end_memory_check();
}
```
24. Enumerated types should be declared with enum.
```
enum category_type{
    MATHEMATICS, SPORT, MUSIC, SLANG, BOTANIC,
    PLURAL, MARINE, HISTORY, THEOLOGY, ZOOLOGY,
    METAPHOR, PSYCHOLOGY, ASTRONOMY, GEOGRAPHY, GRAMMAR,
    MILITARY, PHYSICS, PHILOSOPHY, MEDICAL, THEATER,
    ECONOMY, LAW, ANATOMY, GEOMETRY, BUSINESS,
    PEDAGOGY, TECHNOLOGY, LOGIC, LITERATURE, CINEMA,
    TELEVISION, ARCHITECTURE, TECHNICAL, SOCIOLOGY, BIOLOGY,
    CHEMISTRY, GEOLOGY, INFORMATICS, PHYSIOLOGY, METEOROLOGY,
    MINERALOGY
};
```
25. Every header file should start with
```
#ifndef MATH_DISTRIBUTION_H
#define MATH_DISTRIBUTION_H
```
and end with
```
#endif //MATH_DISTRIBUTION_H
```
26. Use regular package for regular expression handling.
```
	void check_match(char* expression, char* word){
    	Regular_expression_ptr re = create_regular_expression(expression);
    	if (!full_matches(re, word)){
        	printf("Mismatch %s %s\n", re->expression, word);
    	}
    	free_regular_expression(re);
	}
```
27. Do not forget to use const expression for parameters, if they will not be changed in the function.
```
bool frame_lexical_unit_exists(const Frame* frame, const char *synSetId);
```
28. For dynamic string handling, use util package with stringutils.
```
struct string {
    char *s;
    int max_size;
};
typedef struct string String;
typedef String *String_ptr;
String_ptr create_string();
String_ptr create_string2(const char *s);
String_ptr create_string3(const char* s1, const char* s2);
String_ptr create_string4(const char* s1, const char* s2, const char* s3);
```
29. Foor boolean type, use bool.
```
bool check_definition(const Data_set* data_set, const Instance* instance);
```
30. Use xmlparser package for parsing xml files.
```
    Xml_document_ptr doc = create_xml_document("test.xml");
