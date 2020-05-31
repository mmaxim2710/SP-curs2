/* $n08 */
#include "semantics.h"
using namespace std;
void tSM::init(){
	globals.clear();
  	locals.clear();
  	params.clear();
  	scope = 0;
  	ConstExp = true;
	globals["e"] = 
//                properties
		tgName(VAR|DEFINED|BUILT);
	globals["remainder"] = 
//              properties       arity
		tgName(PROC|DEFINED|BUILT, 2);
	globals["abs"] = 
//              properties       arity
		tgName(PROC|DEFINED|BUILT, 1);
    globals["sqrt"] = 
//              properties       arity
		tgName(PROC|DEFINED|BUILT, 1);
	globals["atan"] = 
//              properties       arity
		tgName(PROC|DEFINED|BUILT, 1);
	globals["expt"] = 
//              properties       arity
		tgName(PROC|DEFINED|BUILT, 2);
	globals["log"] = 
//              properties       arity
		tgName(PROC|DEFINED|BUILT, 1);
	globals["pi"] = 
//              properties
		tgName(VAR|DEFINED|BUILT);
	return;}
int tSM::p01(){ //      S -> PROG
	
	bool is_error = false;
 	
	 for(tGlobal::iterator it=globals.begin();
    	it!=globals.end();
    	++it){
		
		std::string temporary_name;
		temporary_name = it->first;

		if (!it->second.test(DEFINED) && 
				it->second.test(USED) && 
				!it->second.test(BUILT) ) {
			
			std::string type;
			if(it->second.test(PROC)) type = "procedure";
			else type = "variable";
			
			//Ошибка 1
			ferror_message += "[!]Procedure application:" + type + " '" + temporary_name + "' not defined, but used" + "\n";
			is_error = true;
			return 1;
		}
		if (it->second.test(DEFINED) && 
			  !it->second.test(USED) && 
			  !it->second.test(BUILT)) {
			
			std::string type;
			if(it->second.test(PROC)) type = "procedure";
			else type = "variable";

			//Ошибка 2
			ferror_message += "[?]Procedure application:" + type + " '" + temporary_name + "' defined, but not used" + "\n";
		}
	}
	return 0;}
int tSM::p02(){ //   PROG -> CALCS
	return 0;}
int tSM::p03(){ //   PROG -> DEFS
	return 0;}
int tSM::p04(){ //   PROG -> DEFS CALCS
	return 0;}
int tSM::p05(){ //      E -> $id
	std::string name = S1->name;
	
	bool is_used = false;
	if ((scope > 1 && locals.count(name)) || (scope > 0 && params.count(name))) is_used = true;

	if (!is_used && globals.count(name)) {
	    
		globals[name].set(USED);
	    
		if (globals[name].test(PROC)) {
			//Ошибка 1
            ferror_message +=
			"[!]Variable application:variable '" +
			name +
            "' conflicts with the procedure in the global space since it was not declared locally" +
			"\n";
            return 1;
	    }
	}
	
	if (!is_used && !globals.count(name)) globals[name] = tgName(VAR|USED);
	
	ConstExp &= globals[name].test(BUILT) &&  globals.count(name);
	
	return 0;}
int tSM::p06(){ //      E -> $int
	return 0;}
int tSM::p07(){ //      E -> $dec
	return 0;}
int tSM::p08(){ //      E -> ADD
	return 0;}
int tSM::p09(){ //      E -> SUB
	return 0;}
int tSM::p10(){ //      E -> DIV
	return 0;}
int tSM::p11(){ //      E -> MUL
	return 0;}
int tSM::p12(){ //      E -> COND
	return 0;}
int tSM::p13(){ //      E -> EASYLET
	return 0;}
int tSM::p14(){ //      E -> CPROC
	ConstExp &= globals[S1->name].test(BUILT) && globals.count(S1->name);
	return 0;}
int tSM::p15(){ //    ADD -> HADD E )
	return 0;}
int tSM::p16(){ //   HADD -> ( +
	return 0;}
int tSM::p17(){ //   HADD -> HADD E
	return 0;}
int tSM::p18(){ //    SUB -> ( - E E )
	return 0;}
int tSM::p19(){ //    DIV -> HDIV E )
	return 0;}
int tSM::p20(){ //   HDIV -> ( /
	return 0;}
int tSM::p21(){ //   HDIV -> HDIV E
	return 0;}
int tSM::p22(){ //    MUL -> HMUL E )
	return 0;}
int tSM::p23(){ //   HMUL -> ( *
	return 0;}
int tSM::p24(){ //   HMUL -> HMUL E
	return 0;}
int tSM::p25(){ //   COND -> HCOND CLAUS )
	return 0;}
int tSM::p26(){ //  HCOND -> ( cond
	return 0;}
int tSM::p27(){ //  HCOND -> HCOND CLAUS
	return 0;}
int tSM::p28(){ //  CLAUS -> ( BOOL E )
	return 0;}
int tSM::p29(){ //EASYLET -> HEASYL E )
	return 0;}
int tSM::p30(){ // HEASYL -> ( let ( )
	return 0;}
int tSM::p31(){ // HEASYL -> HEASYL INTER
	return 0;}
int tSM::p32(){ //    STR -> $str
	return 0;}
int tSM::p33(){ //    STR -> SIF
	return 0;}
int tSM::p34(){ //    SIF -> ( if BOOL STR STR )
	return 0;}
int tSM::p35(){ //  CPROC -> HCPROC )
	string name = S1->name;
 	int count = S1->count;
 	if(scope>1){
    	if(locals.count(name)){
			//Ошибка №1
      		ferror_message+="[!]Procedure application:"
        	" local variable '"+name+
        	"' shadows the procedure!\n";
      		return 1;
    	} // if locals ...
  	} // if scope ...
 	if(scope>0){
    	if(params.count(name)){
		//Ошибка №2
      	ferror_message+="[!]Procedure application:"
        	" parameter '"+name+
        	"' shadows the procedure!\n";
      		return 1;
    	}// if params...
 	}// if scope... 

    tgName& ref = globals[name];
 	do	{
    	if(ref.empty()){
      		ref = tgName(PROC|USED, count);
      		break;
    	}

    	if(!ref.test(PROC)){
			//Ошибка №3
      		ferror_message+="[!]Procedure application:"
        		" '"+ name+
        		"' is not a procedure!\n";
      		return 1;
    	}
		if(ref.arity!=count){
     		std::ostringstream buf;
     		buf << "[!]Procedure application: '" << name << "' "
			//Ошибка №4
        	<< (ref.test(DEFINED) ? "expects " : "has been called already\n\t with ")
			//Ошибка №5
        	<< ref.arity<<" argument"
        	<< (ref.arity!=1 ?"s":"")
        	<< ", given: " << count <<" !\n";
		    ferror_message += buf.str();
     		return 1;
    	}
    	ref.set(USED);
   	} while(false);

   	if(ConstExp && !ref.test(BUILT)) {
    	ConstExp = false;
	}
	return 0;}
int tSM::p36(){ // HCPROC -> ( $id
	S1->name = S2->name;
	S1->count = 0;
	return 0;}
int tSM::p37(){ // HCPROC -> HCPROC E
	++(S1->count);
	return 0;}
int tSM::p38(){ //   BOOL -> $bool
	return 0;}
int tSM::p39(){ //   BOOL -> $idq
	std::string name;
	name = S1->name;

	bool is_used = false;
	if ((scope > 1 && locals.count(name)) || (scope > 0 && params.count(name))) is_used = true;

	if (is_used == false) {
		//Ошибка 1
		ferror_message +=
		"[!]Bool variable application:  '" +
		name +
        "' not declared in this scope" + "\n";
		return 1;
	}


	ConstExp &= globals[name].test(BUILT) && globals.count(name);
	return 0;}
int tSM::p40(){ //   BOOL -> REL
	return 0;}
int tSM::p41(){ //   BOOL -> CPRED
	return 0;}
int tSM::p42(){ //  CPRED -> HCPRED )
	string name = S1->name;
 	int count = S1->count;
	int types = S1->types;
 	if(scope>0){
    	if(params.count(name)){
		//Ошибка №1
      	ferror_message+="[!]Predicate application: '" + name + "' shadow the predicate!\n";
      		return 1;
    	}// if params...
 	}// if scope... 

    tgName& ref = globals[name];
 	do	{
    	if(ref.empty()){
      		ref = tgName(PROC|USED, count, types);
      		break;
    	}
		if(ref.arity!=count) {
     		std::ostringstream buf;
     		buf<<"[!]Predicate application: '"<< name << "' "
			//Ошибка №2
        	<< (ref.test(DEFINED) ? "expects " : "has been called already\n\t with ")
			//Ошибка №3
        	<< ref.arity<<" argument"
        	<< (ref.arity!=1 ?"s":"")
        	<< ", given: " << count <<" !\n";
		    ferror_message += buf.str();
     		return 1;
    	}
		if (ref.types != types) {
			std::ostringstream buf;
			std::string prev_types = [&ref] () {
				std::string result = "(";
				for (int i = 0; i < ref.arity; ++i) {
					if (i != 0) {
						result += " ";
					}
					result += ((ref.types & (1 << i)) ? "BOOL" : "E");
				}
                return result + ")";
			} ();
			std::string cur_types = [&count, &types] () {
				std::string result = "(";
				for (int i = 0; i < count; ++i) {
					if (i != 0) {
						result += " ";
					}
					result += ((types & (1 << i)) ? "BOOL" : "E");
				}
				return result + ")";
			} ();
     		buf<<"[!]Predicate application: '"<< name << "' "
			//Ошибка №4
        	<< (ref.test(DEFINED) ? "expects " : "has been called already\n\t with ")
			//Ошибка №5
        	<< prev_types <<" types"
        	<< ", given: " << cur_types <<" !\n";
		    ferror_message += buf.str();
     		return 1;
		}
    	ref.set(USED);
   	} while(false);

   	if(ConstExp && !ref.test(BUILT)) {
    	ConstExp = false;
	}
	return 0;}
int tSM::p43(){ // HCPRED -> ( $idq
	S1->name = S2->name;
	S1->count = 0;
	return 0;}
int tSM::p44(){ // HCPRED -> HCPRED ARG
	S1->types = S1->types | (S2->types << S1->count);
	S1->count++;
	return 0;}
int tSM::p45(){ //    ARG -> E
	S1->types = 0;
	return 0;}
int tSM::p46(){ //    ARG -> BOOL
	S1->types = 1;
	return 0;}
int tSM::p47(){ //    REL -> ( = E E )
	return 0;}
int tSM::p48(){ //    REL -> ( >= E E )
	return 0;}
int tSM::p49(){ //    SET -> HSET E )
ConstExp = false;
	return 0;}
int tSM::p50(){ //   HSET -> ( set! $id
	std::string name = S3->name;
	
	bool is_used = false;
	if ((scope > 1 && locals.count(name)) || (scope > 0 && params.count(name))) is_used = true;

	if (globals.count(name) == false && is_used == false) globals[name] = tgName(VAR|USED);

	return 0;}
int tSM::p51(){ //DISPSET -> ( display E )
	return 0;}
int tSM::p52(){ //DISPSET -> ( display BOOL )
	return 0;}
int tSM::p53(){ //DISPSET -> ( display STR )
	return 0;}
int tSM::p54(){ //DISPSET -> ( newline )
	return 0;}
int tSM::p55(){ //DISPSET -> SET
	return 0;}
int tSM::p56(){ //  INTER -> DISPSET
	ConstExp = false;
	return 0;}
int tSM::p57(){ //  INTER -> E
	return 0;}
int tSM::p58(){ //  CALCS -> CALC
	return 0;}
int tSM::p59(){ //  CALCS -> CALCS CALC
	return 0;}
int tSM::p60(){ //   CALC -> E
	return 0;}
int tSM::p61(){ //   CALC -> BOOL
	return 0;}
int tSM::p62(){ //   CALC -> STR
	return 0;}
int tSM::p63(){ //   CALC -> DISPSET
	return 0;}
int tSM::p64(){ //   DEFS -> DEF
	return 0;}
int tSM::p65(){ //   DEFS -> DEFS DEF
	return 0;}
int tSM::p66(){ //    DEF -> PRED
	return 0;}
int tSM::p67(){ //    DEF -> VAR
	return 0;}
int tSM::p68(){ //    DEF -> PROC
	return 0;}
int tSM::p69(){ //   PRED -> HPRED BOOL )
	params.clear();
	ConstExp = true;
	scope = 0;
	return 0;}
int tSM::p70(){ //  HPRED -> PDPAR )
	std::string name = S1->name;

    tgName& ref = globals[name];

	int count = S1->count;
	int types = S1->types;
 	do	{
		if (ref.test(DEFINED) == true) {
			//Ошибка 1
			ferror_message += "[!]Procedure definition: multiple definition of " + name + " procedure\n";
			return 1;
		}

    	if(ref.empty()){
      		ref = tgName(PROC|DEFINED, count, types);
      		break;
    	}

		if(ref.arity!=count) {
     		std::ostringstream buf;
     		buf<<"[!]Predicate definition: '"<< name << "' "
			//Ошибка №2
        	<< "has been called already\n\t with "
        	<< ref.arity<<" argument"
        	<< (ref.arity!=1 ?"s":"")
        	<< ", given: " << count <<" !\n";
		    ferror_message += buf.str();
     		return 1;
    	}
		if (ref.types != types) {
			std::ostringstream buf;
			std::string prev_types = [&ref] () {
				std::string result;
				for (int i = 0; i < ref.arity; ++i) {
					if (i != 0) {
						result += " ";
					}
					result += ((ref.types & (1 << i)) ? "bool" : "number");
				}
				return result;
			} ();
			std::string cur_types = [&count, &types] () {
				std::string result;
				for (int i = 0; i < count; ++i) {
					if (i != 0) {
						result += " ";
					}
					result += ((types & (1 << i)) ? "bool" : "number");
				}
				return result;
			} ();
			
     		buf<<"[!]Predicate definition: '"<< name << "' "
			//Ошибка №3
        	<< "has been called already\n\t with "
        	<< prev_types <<" types"
        	<< ", given: " << cur_types <<" !\n";
		    ferror_message += buf.str();
     		return 1;
		}
    	ref.set(DEFINED);
   	} while(false);

	scope = 1;
	return 0;}
int tSM::p71(){ //  PDPAR -> ( define ( $idq
	S1->name = S4->name;
	S1->count = 0;
	S1->types = 0;
	return 0;}
int tSM::p72(){ //  PDPAR -> PDPAR $idq
	S1->types = S1->types | (1 << S1->count);
	if(params.count(S2->name) == true){
    	//Ошибка №1
    	ferror_message +=
    	"[!]Procedure definition: in '" +
		S1->name +
        "' duplicate parameter identifier '" +
		S2->name +"'!\n";
    	return 1;
    } else {
    	params.insert(S2->name);
 		++(S1->count);
	}
	return 0;}
int tSM::p73(){ //  PDPAR -> PDPAR $id
	if(params.count(S2->name) == true){
    	//Ошибка №1
    	ferror_message +=
        "[!]Procedure definition: in '" +
		S1->name+
        "' duplicate parameter identifier '" +
		S2->name +
		"'!\n";
      return 1;
    } else {
    	params.insert(S2->name);
 		++(S1->count);
	}
	return 0;}
int tSM::p74(){ //    VAR -> VARDCL E )
    std::string name = S1->name;

	if (ConstExp == false) {
		//Ошибка №1
		ferror_message +=
		"[!]Variable definition: non-const definition of " +
		name +
		"variable \n";
		return 1;
	}
	ConstExp = true;
	return 0;}
int tSM::p75(){ // VARDCL -> ( define $id
S1->name = S3->name;
	std::string name = S1->name;
	tgName& ref = globals[name];
	do {
		if (ref.empty() == true) {
			globals[name] = tgName(VAR|DEFINED);
			break;
		}

		if (ref.test(DEFINED) == true) {
			//Ошибка №1
			ferror_message +=
			"[!]Variable definition: multiple definition of " +
			name +
			" variable\n";
			return 1;
		} 

		if (ref.test(PROC) == true) {
			//Ошибка №2
			ferror_message +=
			"[!]Variable definition: variable " +
			name +
			"already used as procedure\n";
			return 1;
		}

        ref.set(DEFINED);
	} while (false);
	return 0;}
int tSM::p76(){ //   PROC -> HPROC BLOCK )
	params.clear();
	scope = 0;
	ConstExp = true;
	return 0;}
int tSM::p77(){ //   PROC -> HPROC E )
	params.clear();
	scope = 0;
	ConstExp = true;
	return 0;}
int tSM::p78(){ //  HPROC -> PCPAR )
	int count = S1->count;
	std::string name = S1->name;
	tgName& ref = globals[S1->name];
	
	do	{
    	if(ref.empty()){
      		ref = tgName(PROC|DEFINED, count);
      		break;
    	}

		if(ref.test(PROC) == false){
			//Ошибка №1
      		ferror_message +=
			"[!]Procedure definition: '" +
			name +
        	"' used as global variable\n";
      		return 1;
    	}

		if (ref.test(DEFINED) == true) {
			//Ошибка №2
			ferror_message +=
			"[!]Procedure definition: multiple definition of " +
			name +
			" procedure \n";
			return 1;
		}
    	
		if(ref.arity != count){
			//Ошибка №3
     		std::ostringstream buf;
     		buf<<"[!]Procedure application: '"
			<< name
			<< "' "
        	<< "has been called already\n\t with "
        	<< ref.arity<<" argument"
        	<< (ref.arity!=1 ?"s":"")
        	<< ", given: " << count <<" !\n";
		    ferror_message += buf.str();
     		return 1;
    	}
		ref.set(DEFINED);
   	} while(false);
	
	scope = 1;
	
	return 0;}
int tSM::p79(){ //  HPROC -> HPROC INTER
	return 0;}
int tSM::p80(){ //  PCPAR -> ( define ( $id
	S1->count = 0;
	S1->name = S4->name;
	return 0;}
int tSM::p81(){ //  PCPAR -> PCPAR $id
  if(params.count(S2->name) == true){
    //Ошибка №1
      ferror_message+=
        "[!]Procedure definition: in '"
        +S1->name+
        "' duplicate parameter '"
        +S2->name+"'!\n";
      return 1;
    }
    params.insert(S2->name);
 	++S1->count;
	return 0;}
int tSM::p82(){ //  BLOCK -> HBLOCK E )
	scope = 1;
	locals.clear();
	return 0;}
int tSM::p83(){ // HBLOCK -> BLVAR )
	scope = 2;
	return 0;}
int tSM::p84(){ // HBLOCK -> HBLOCK INTER
	return 0;}
int tSM::p85(){ //  BLVAR -> ( let ( LOCDEF
	return 0;}
int tSM::p86(){ //  BLVAR -> BLVAR LOCDEF
	return 0;}
int tSM::p87(){ // LOCDEF -> ( $id E )
std::string name = S2->name;

	if (locals.count(name) == true) {
		//Ошибка №1
		ferror_message +=
        "[!]Variable definition: local variable " +
		name +
		" redefinition\n";
      	return 1;
	}
	locals.insert(name);
	return 0;}
//_____________________
int tSM::p88(){return 0;} int tSM::p89(){return 0;} 
int tSM::p90(){return 0;} int tSM::p91(){return 0;}
int tSM::p92(){return 0;} int tSM::p93(){return 0;} 
int tSM::p94(){return 0;} int tSM::p95(){return 0;} 
int tSM::p96(){return 0;} int tSM::p97(){return 0;} 
int tSM::p98(){return 0;} int tSM::p99(){return 0;} 
int tSM::p100(){return 0;} int tSM::p101(){return 0;} 
int tSM::p102(){return 0;} int tSM::p103(){return 0;} 
int tSM::p104(){return 0;} int tSM::p105(){return 0;} 
int tSM::p106(){return 0;} int tSM::p107(){return 0;} 
int tSM::p108(){return 0;} int tSM::p109(){return 0;} 
int tSM::p110(){return 0;} 
