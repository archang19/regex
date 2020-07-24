//
//  formatter.h
//  regex
//
//  Created by Andrew Chang on 7/23/20.
//  Copyright © 2020 Andrew Chang. All rights reserved.
//

#ifndef formatter_h
#define formatter_h

#include <string>

std::string to_post_fix (std::string exp);
std::string insert_explicit_concat_operator(std::string exp);

#endif /* formatter_h */
