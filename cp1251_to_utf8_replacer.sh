find . -name '*.hpp' | while read i
    do
    if [[ $(file -b --mime-encoding "$i") == "iso-8859-1" ]]; then        
        iconv -f cp1251 -t UTF-8 "$i" >tmp
        mv tmp "$i"
        echo $i " encoded from cp1251 to utf-8"
    else
        if [[ $(file -b --mime-encoding "$i") != "us-ascii" 
            && $(file -b --mime-encoding "$i") != "utf-8" ]]; then    
            echo $i " has unknown" $(file -b --mime-encoding "$i") "charset"
        else
            echo $i already_encoded
        fi
    fi
    done

find . -name '*.cpp' | while read i
    do
    if [[ $(file -b --mime-encoding "$i") == "iso-8859-1" ]]; then        
        iconv -f cp1251 -t UTF-8 "$i" >tmp
        mv tmp "$i"
        echo $i " encoded from cp1251 to utf-8"
    else
        if [[ $(file -b --mime-encoding "$i") != "us-ascii" 
            && $(file -b --mime-encoding "$i") != "utf-8" ]]; then    
            echo $i " has unknown" $(file -b --mime-encoding "$i") "charset"
        else
            echo $i already_encoded
        fi
    fi
    done

find . -name '*.c' | while read i
    do
    if [[ $(file -b --mime-encoding "$i") == "iso-8859-1" ]]; then        
        iconv -f cp1251 -t UTF-8 "$i" >tmp
        mv tmp "$i"
        echo $i " encoded from cp1251 to utf-8"
    else
        if [[ $(file -b --mime-encoding "$i") != "us-ascii" 
            && $(file -b --mime-encoding "$i") != "utf-8" ]]; then    
            echo $i " has unknown" $(file -b --mime-encoding "$i") "charset"
        else
            echo $i already_encoded
        fi
    fi
    done

find . -name '*.h' | while read i
    do
    if [[ $(file -b --mime-encoding "$i") == "iso-8859-1" ]]; then        
        iconv -f cp1251 -t UTF-8 "$i" >tmp
        mv tmp "$i"
        echo $i " encoded from cp1251 to utf-8"
    else
        if [[ $(file -b --mime-encoding "$i") != "us-ascii" 
            && $(file -b --mime-encoding "$i") != "utf-8" ]]; then    
            echo $i " has unknown" $(file -b --mime-encoding "$i") "charset"
        else
            echo $i already_encoded
        fi
    fi
    done
