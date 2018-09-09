ReadMe.txt

Inorder to understand how Parser works, see the ParserBlog: 
http://ecs.syr.edu/faculty/fawcett/handouts/webpages/BlogParser.htm

This Parser demo code is packaged so that you only need to change 3 files to support 
any parsing application:
- ActionsAndRules.h:
  Defines all of the parser's IRule and IAction derived classes.
  - Each rule detects some grammatical construct, e.g., function definition or data declaration.
  - Each action does something when the input SemiExp matches the rule, e.g., add info to Repository 
    or print something
- ConfigParser.h and ConfigParser.cpp
  Constructs instances of tokenizer, parser, all the rules, and all the actions.  
  The ConfigParserToConsole owns all of these objects, holds onto them for the entire exection and then
  destroys them when it goes out of scope.

You might also add to Utilities to provide small helper functions and classes as needed by
your implementation.