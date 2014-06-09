#Focus Tracking

2014 spring OOP course project.

##Build
On Mac OS X, install dependencies (using brew): 
 
	brew install curl libmicrohttpd pcre sqlite cmake autoconf automake bison flex libtool pkg-config

Than just make: 

    make
    
Unzip `model.7z` to `bin` folder.  
Initialize database:

	sqlite3 bin/crawling.db <scripts/init.sql
    
##Run

	cd bin
	./focus
	
Open `http://localhost:8888/` to view result.

##Documentation

Technical Report: `documentation/tech_report.pdf`

User Manual: `documentation/user_manual.pdf`

Middle Report: `documentation/mid_report.pdf`

Final Report (Keynote): `documentation/final_report.key`