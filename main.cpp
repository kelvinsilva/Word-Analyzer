//LINKER settings -lComDlg32
//April 14 2013
//Open source and free to distribute to anyone! Share alike!


#include <windows.h>
#include <string>
#include <process.h> //_beginthreadex
#include <fstream>
//Uncomment if you wish to debug. Uncomment things in the worker thread that deal with cout<< to debug also
// #include <iostream>
#include <sstream>
#include <algorithm> // transform()
#include <cctype>
#include <map> //dictionaryFormal,Informal

#include "resource.h"

HINSTANCE hInst;
//GetOpenFileName
OPENFILENAME ofn;

//the PROPER way to do namespace directives
using std::string;
using std::ifstream;
using std::stringstream;
using std::map;

unsigned int __stdcall process_text(void*);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

struct messageParams{

    string filePath;
    bool textOrbox;

};

struct verdict{

    long MaleFormal, MaleInformal, FemaleFormal, FemaleInformal;
    int differenceFormal, differenceInformal;
    double percentFORMAL, percentINFORMAL;
    int wordct;
    int weakFORMAL, weakINFORMAL;


};

verdict VERDICT{

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

map<string, int> DictionaryFormal, DictionaryInformal;

//ATM its a global variable, since i may be planning on doing something else with this in the future.
string contents;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;

    DictionaryInformal["actually"]= -49;
	DictionaryInformal["am"]= -42;
	DictionaryInformal["as"]= 37;
	DictionaryInformal["because"]= -55;
	DictionaryInformal["but"]= -43;
	DictionaryInformal["ever"]= 21;
	DictionaryInformal["everything"]= -44;
	DictionaryInformal["good"]= 31;
	DictionaryInformal["has"]= -33;
	DictionaryInformal["him"]= -73;
	DictionaryInformal["if"]= 25;
	DictionaryInformal["in"]= 10;
	DictionaryInformal["is"]= 19;
	DictionaryInformal["like"]= -43;
	DictionaryInformal["more"]= -41;
	DictionaryInformal["now"]= 33;
	DictionaryInformal["out"]= -39;
	DictionaryInformal["since"]= -25;
	DictionaryInformal["so"]= -64;
	DictionaryInformal["some"]= 58;
	DictionaryInformal["something"]= 26;
	DictionaryInformal["the"]= 17;
	DictionaryInformal["this"]= 44;
	DictionaryInformal["too"]= -38;
	DictionaryInformal["well"]= 15;

	DictionaryFormal["a"]= 6;
	DictionaryFormal["above"]= 4;
	DictionaryFormal["and"]= -4;
	DictionaryFormal["are"]= 28;
	DictionaryFormal["around"]= 42;
	DictionaryFormal["as"]= 23;
	DictionaryFormal["at"]= 6;
	DictionaryFormal["be"]= -17;
	DictionaryFormal["below"]= 8;
	DictionaryFormal["her"]= -9;
	DictionaryFormal["hers"]= -3;
	DictionaryFormal["if"]= -47;
	DictionaryFormal["is"]= 8;
	DictionaryFormal["it"]= 6;
	DictionaryFormal["many"]= 6;
	DictionaryFormal["me"]= -4;
	DictionaryFormal["more"]= 34;
	DictionaryFormal["myself"]= -4;
	DictionaryFormal["not"]= -27;
	DictionaryFormal["said"]= 5;
	DictionaryFormal["she"]= -6;
	DictionaryFormal["should"]= -7;
	DictionaryFormal["the"]= 7;
	DictionaryFormal["these"]= 8;
	DictionaryFormal["to"]= 2;
	DictionaryFormal["was"]= -1;
	DictionaryFormal["we"]= -8;
	DictionaryFormal["what"]= 35;
	DictionaryFormal["when"]= -17;
	DictionaryFormal["where"]= -18;
	DictionaryFormal["who"]= 19;
	DictionaryFormal["with"]= -52;
	DictionaryFormal["your"]= -17;

    // The user interface is a modal dialog box
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DialogProc);
}


BOOL CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            /*
             * TODO: Add code to initialize the dialog.
             */
            return TRUE;

        case WM_CLOSE:
            EndDialog(hwnd, 0);

            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case IDC_BOX:{
                    //first radio button choice, not really anything is done here, decision is made when generate is pressed
                 }
                 break;

                 case IDC_FILE:{
                    //

                 }break;

                case IDC_BRWS:{

                    char szFileName[MAX_PATH] = "";

                    ZeroMemory(&ofn, sizeof(ofn));

                        ofn.lStructSize = sizeof(ofn);
                        ofn.hwndOwner = hwnd;
                        ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                        ofn.lpstrFile = szFileName;
                        ofn.nMaxFile = MAX_PATH;
                        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                        ofn.lpstrDefExt = "txt";

                        if(GetOpenFileName(&ofn)){
                            SetDlgItemText(hwnd, IDC_BRWSEDIT, ofn.lpstrFile);

                        }


                    return TRUE;
                }

                case IDC_ANAL:{

                        messageParams* threadParams = new messageParams;


                        if (BST_CHECKED == IsDlgButtonChecked(hwnd, IDC_BOX)){

                                threadParams->textOrbox = false;

                                contents = "";
                                int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_BOXEDIT));
                                char* content = new char[len+1];
                                GetDlgItemText(hwnd, IDC_BOXEDIT, content, len+1);

                                contents.append(content);
                                delete[] content;


                        }else if (BST_CHECKED == IsDlgButtonChecked(hwnd, IDC_FILE)){
                                contents = "";
                                threadParams->textOrbox = true;

                        }else{

                             MessageBox(NULL, "No Radio Button selected!\nDefault analyzing from textbox!", "Warning!", MB_ICONASTERISK);
                                threadParams->textOrbox = false;

                                contents = "";
                                int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_BOXEDIT));
                                char* content = new char[len+1];
                                GetDlgItemText(hwnd, IDC_BOXEDIT, content, len+1);

                                contents.append(content);
                                delete[] content;
                        }



                        int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_BRWSEDIT));
                        char* fPath = new char[len+1];
                        GetDlgItemText(hwnd, IDC_BRWSEDIT, fPath, len+1);

                        threadParams->filePath.append(fPath);
                        //string *filepath = new string;
                        //filepath->append(fPath);

                        HANDLE hWorker = (HANDLE)_beginthreadex(NULL, 0, &process_text, threadParams , 0, NULL);
                        MessageBox(NULL, "Please Wait for processing to complete!\r\n Press OK.", "Notification!", MB_ICONASTERISK);
                        //Wait for the worker thread to finish!
                        WaitForSingleObject(hWorker,INFINITE);
                        MessageBox(NULL, "Processing Completed!", "Notification!", MB_ICONASTERISK);


                        //Setup a stringstream to format our text for each edit control.
                        stringstream BOXBUFFER;

                        if (VERDICT.wordct < 300){
                            BOXBUFFER << "Word count is less than 300 words!\r\nWord Count: " << VERDICT.wordct;
                        }else BOXBUFFER << "Word Count: " << VERDICT.wordct;

                        SetDlgItemText(hwnd, IDC_STAT, BOXBUFFER.str().c_str() );

                        BOXBUFFER.str("");
                        BOXBUFFER << "Genre Informal\r\nFemale = " << VERDICT.FemaleInformal << "\r\nMale = " << VERDICT.MaleInformal
                                  <<"\r\nDifference: " << VERDICT.differenceInformal << "\r\nPercentage = " << VERDICT.percentINFORMAL;

                        if(VERDICT.MaleInformal > VERDICT.FemaleInformal)
                        BOXBUFFER <<"\r\nVerdict: MALE";

                        else if (VERDICT.MaleInformal < VERDICT.FemaleInformal)
                        BOXBUFFER <<"\r\nVerdict: FEMALE";

                        else
                        BOXBUFFER << "\r\nVerdict: UNKNOWN";

                        if (VERDICT.weakINFORMAL)
                        BOXBUFFER << "\r\nWeak: Emphasis could indicate european origins?";


                        SetDlgItemText(hwnd, IDC_INFORMAL, BOXBUFFER.str().c_str());

                        BOXBUFFER.str("");
                        BOXBUFFER << "Genre Formal\r\nFemale = " << VERDICT.FemaleFormal << "\r\nMale = " << VERDICT.MaleFormal
                                  << "\r\nDifference: " << VERDICT.differenceFormal << "\r\nPercentage = " << VERDICT.percentFORMAL;

                        if(VERDICT.MaleFormal > VERDICT.FemaleFormal)
                        BOXBUFFER << "\r\nVerdict: MALE";

                        else if(VERDICT.MaleFormal < VERDICT.FemaleFormal)
                        BOXBUFFER << "\r\nVerdict: FEMALE";

                        else
                        BOXBUFFER << "\r\nVerdict: UNKNOWN";

                        if (VERDICT.weakFORMAL)
                        BOXBUFFER << "\r\nWeak: Emphasis could indicate european origins?";

                        SetDlgItemText(hwnd, IDC_FORMAL, BOXBUFFER.str().c_str());

                        delete []fPath;
                        delete threadParams;
                }

                    return TRUE;
            }
    }

    return FALSE;
}


unsigned int __stdcall process_text(void* args){

//    params threadParams = *(struct params*)args;

    messageParams currentParams = *(messageParams*)args;


    extern string contents;
    extern verdict VERDICT;
    VERDICT = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    extern map<string, int> DictionaryFormal, DictionaryInformal;
    string::iterator it;


    if (currentParams.textOrbox){
        ifstream intext(currentParams.filePath.c_str() ,std::ios::in | std::ios::binary);

        if(!intext.is_open()){
            MessageBox(NULL, "Error! File could not be read!", "Exception!", MB_ICONERROR);
            return 0;
        }

        if (intext){
                intext.seekg(0, std::ios::end);
                contents.resize(intext.tellg());
                intext.seekg(0, std::ios::beg);
                intext.read(&contents[0], contents.size());
        }
        intext.close();

    }


      for (it = contents.begin(); it < contents.end(); it++){

                  if (*it == '.' || *it == '\n' || *it == '\t'){
                        contents.replace(it, it+1, " ");
                        it--;
                  }else if (!isalpha(*it) && *it != ' '){
                        contents.erase(it);
                            it--;
                  }


      }
      std::transform(contents.begin(), contents.end(), contents.begin(), ::tolower);
/*
      for (it = contents.begin(); it< contents.end(); it++){
            if (*it == ' '){
                VERDICT.wordct++;
            }
      }*/

      //tokenize and parse
        map<string, int>::iterator itFormal, itInformal;


        string buffer;
        stringstream ss(contents);


      while (ss>>buffer){
        itFormal = DictionaryFormal.find(buffer);
        itInformal = DictionaryInformal.find(buffer);

        if( itInformal != DictionaryInformal.end()){

            if( (itInformal->second) > 0 )
                VERDICT.MaleInformal += (long)itInformal->second;

            else
                VERDICT.FemaleInformal -= (long)itInformal->second;
        }

        if( itFormal != DictionaryFormal.end()){

            if ( (itFormal->second) > 0 ){
                VERDICT.MaleFormal += (long)itFormal->second;

            }else
                VERDICT.FemaleFormal -= (long)itFormal->second;
        }

        //Increase wordcount everytime we evaluate a word.
        VERDICT.wordct++;

      }
        //Gender is determined if the formality enumeration is greater than the other.

      VERDICT.differenceFormal = VERDICT.MaleFormal - VERDICT.FemaleFormal;
      VERDICT.differenceInformal = VERDICT.MaleInformal - VERDICT.FemaleInformal;

      if ((VERDICT.MaleFormal + VERDICT.FemaleFormal) > 0)
            VERDICT.percentFORMAL = VERDICT.MaleFormal * 100.0 / (VERDICT.MaleFormal + VERDICT.FemaleFormal);
      else
            VERDICT.percentFORMAL = 0.001;


      if ((VERDICT.MaleInformal + VERDICT.FemaleInformal) > 0)
            VERDICT.percentINFORMAL = VERDICT.MaleInformal * 100.0 / (VERDICT.MaleInformal + VERDICT.FemaleInformal);
      else
            VERDICT.percentINFORMAL = 0;

    //Use of ternary operator. IF condition then B else C ----> condition ? b : c
      VERDICT.weakINFORMAL = VERDICT.percentINFORMAL > 40 && VERDICT.percentINFORMAL < 60 ? 1 : 0;
      VERDICT.weakFORMAL = VERDICT.percentFORMAL > 40  && VERDICT.percentFORMAL < 60 ? 1 : 0;



        //Uncomment the below to debug. Note the max size, do not load an encyclopedia that is greater than max_size()
      //std::cout << "Female Formal: " << VERDICT.FemaleFormal << "\nFemaleInformal: " << VERDICT.FemaleInformal << "\nMALE formal: " << VERDICT.MaleFormal << "\nMale Informal: " << VERDICT.MaleInformal << "\nPercentFormal: " << VERDICT.percentFORMAL << "\nPercentInformal: " << VERDICT.percentINFORMAL;
      //std::cout << contents.max_size();
      //std::cout << contents;


      return 0;

}
