/* -*- mode: c++; indent-tabs-mode: nil -*-
 *
 *  $Id: persistent_vector_test.cpp,v 1.1 2006/10/09 07:08:01 Paul Pedersen Exp $
 *
 *  Copyright 2006-2007 Paul Pedersen.  All Rights Reserved.
 *
 *	Author: Paul Pedersen
 *
 */

#include "fxvector.h"

#include <iostream>
#include <fstream>

#include "xqp_exception.h"
#include "tokenbuf.h"

using namespace std;
using namespace xqp;


int main(int argc, char* argv[])
{
  try {
		fxvector<char> * fxvec_p;
		if (argc > 1) {
    	fxvec_p = new fxvector<char>(string(argv[1],0,strlen(argv[0])));
		}
		else {
    	fxvec_p = new fxvector<char>();
		}
		fxvector<char> & fxvec = *fxvec_p;
  
    // check metadata
    cout << "v.size() = " << fxvec.size() << endl;
    cout << "v.capacity() = " << fxvec.capacity() << endl;

    // command loop
		string cmdline, cmd, arg1, arg2;
		uint32_t arg_count = 0;

    while (true) {
			getline(cin, cmdline);
			if (cmdline.length()==0) continue;
			
			tokenbuf tokbuf(cmdline.c_str()," ");
			tokbuf.set_lowercase(false);
			tokbuf.set_return_delims(false);

			tokenbuf::token_iterator it = tokbuf.begin();
			tokenbuf::token_iterator end = tokbuf.end();
			
			cmd = *it;
			if (it!=end) { ++it; arg1 = *it; ++arg_count; }
			if (it!=end) { ++it; arg2 = *it; ++arg_count; }

			cout << "cmd = " << cmd << endl;
			if (arg_count>0) cout << "arg1 = " << arg1 << endl;
			if (arg_count>1) cout << "arg2 = " << arg2 << endl;

      if (cmd=="quit" || cmd=="exit") {
				break;
			}
      else if (cmd=="capacity") {
        cout << "capacity = " << fxvec.capacity() << endl;
      }
      else if (cmd=="help") {
        cout << "at n          - return the n-th element of v\n";
        cout << "cap           - print capacity\n";
        cout << "clear         - clear the vector\n";
				cout << "cp STRING     - raw byte copy\n";
        cout << "help          - this help function\n";
        cout << "iter          - run the vector iterator\n";
				cout << "load FNAME    - load file\n";
        cout << "size          - return the size of this vector\n";
				cout << "uniq          - set the entries to a uniq sequence\n";
			}
			else if (cmd=="cp") {
				uint32_t n = arg1.length();
				unsigned char *p = fxvec.raw_copy((unsigned char*)arg1.c_str(), n);
				cout << string((char*)p,0,n);
      }
      else if (cmd=="iter") {
        fxvector<char>::const_iterator it = fxvec.begin();
        for (; it!=fxvec.end(); ++it) {
          cout << *it << endl;
        }
      }
      else if (cmd=="uniq") {
        for (unsigned k=0; k<fxvec.size(); ++k) {
          fxvec[k] = k;
        }
      }
      else if (cmd=="at") {
        unsigned n = strtol(arg1.c_str(),NULL,10);
        char e = fxvec[n];
        cout << "v[" << n << "] = " << e << endl;
      }
      else if (cmd=="size") {
        cout << "size = " << fxvec.size() << endl;
      }
      else if (cmd=="clear") {
				fxvec.clear();
        cout << "size = " << fxvec.size() << endl;
      }
			else if (cmd=="load") {
				ifstream in(arg1.c_str(),ios::in);
				string line;
				while (!in.eof()) {
					getline(in, line);
					fxvec.push_back(strtoul(line.c_str(),NULL,10));
				}
			}
    }
  } catch (xqp_exception& e) {
    cout << "Application exception: " << e.what() << '\t' << e.get_msg() << endl;
  }
  return 0;
}

