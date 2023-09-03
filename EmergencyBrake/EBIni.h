//
// Generated on 2023/09/02 by inimoni 
//

/*
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Documentation
  Struct = Section name, Field = Key name.
  The function used is only a declaration, load(), and save().
  The constructor automatically reads initialization file [initFileName].
  Please rewrite [initFileName] in the source freely.
  It becomes Windows folder when there is not path. The relative path can be used.

Example
#include "SampleIni.h"
void CTestDlg::OnButton1()
{
    //initFileName = "./sample.ini";
    SampleIni data1;                   // Open ./sample.ini
    int v = data1.Sample.value;        // [Sample]-value Get
    data1.Sample.value = v+1;          // [Sample]-value Modify
    data1.save();                      // Save ./Sample.ini (Opened file)

    SampleIni data2("sample.ini");     // Open C:/WINDOWS/Sample.ini
    data2.load("./sample.ini");        // Open Specified file.
    data2.save();                      // Save Opened file (Making at not being)
}

Supplementation
  Reading and writing becomes the unit of the file.
  [initFileName] is the same as the argument of GetPrivateProfileString() of SDK.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#ifndef EBINI_H
#define EBINI_H

#include <string>
using namespace std;

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#ifdef _UNICODE
typedef std::wstring string_t;
#else
typedef std::string string_t;
#endif

namespace inimoni {
    template<class T>
    static bool inirw(int is_read, string_t& fname, string_t sec, string_t key, T& val_t);
};

/**
    EB.ini input output class.
*/
class EBIni
{
public:

    EBIni(string_t fname = _T(""))
    {
        initFileName = _T("./EB.ini"); //Default file.    
        init();
        load(fname);
    }

    /**
        INI file is read.
        @param fname Filename
        When there is not a file, It becomes initFileName.
        When there is not a pass, It becomes Windows folder.
    */
    bool load(string_t fname = _T(""))
    {
        if (fname.empty()) {
            fname = initFileName;
        }
        loadFileName = fname;
        WIN32_FIND_DATA fd;
        HANDLE h = ::FindFirstFile(fname.c_str(), &fd);
        if (h != INVALID_HANDLE_VALUE) {
            iniRW(fname, 1);
        }
        return (h != INVALID_HANDLE_VALUE);
    }

    /**
        It writes it in the INI file.
        @param fname Filename
        When there is not a file, It becomes open file.
        When there is not a pass, It becomes Windows folder.
    */
    bool save(string_t fname = _T(""))
    {
        if (fname.empty()) {
            fname = loadFileName;
        }
        iniRW(fname, 0);
        return true;
    }

public:

    // Index
    struct _Index
    {
        int      Blinking;                //点滅灯
        int      Lighting;                //点灯灯
        int      Buzzer;                  //EBブザー
        int      SW;                      //EBスイッチ
    } Index;

    // SN
    struct _SN
    {
        int      UseSN;                   //ATS-S表示灯と兼用
    } SN;

    // MetroPI
    struct _MetroPI
    {
        int      UseMetroPI;              //メトロPIと兼用
        int      MasConKey;               //マスコンキー
        int      Empty;                   //各社局での使用可否
        int      Metro;
        int      Tobu;
        int      Tokyu;
        int      Seibu;
        int      Sotetsu;
        int      JR;
        int      Odakyu;
        int      Toyo;
    } MetroPI;

    // Emulate
    struct _Emulate
    {
        int      Pitch;                   //点滅間隔
    } Emulate;

    // Key
    struct _Key
    {
        string_t Switch;                  //スイッチ
    } Key;

protected:

    string_t initFileName;
    string_t loadFileName;

    bool iniRW(string_t f, int r)
    {
        string_t s;

        s = _T("Index");
        inimoni::inirw(r, f, s, _T("Blinking         "), Index.Blinking);
        inimoni::inirw(r, f, s, _T("Lighting         "), Index.Lighting);
        inimoni::inirw(r, f, s, _T("Buzzer           "), Index.Buzzer);
        inimoni::inirw(r, f, s, _T("SW               "), Index.SW);

        s = _T("SN");
        inimoni::inirw(r, f, s, _T("UseSN            "), SN.UseSN);

        s = _T("MetroPI");
        inimoni::inirw(r, f, s, _T("UseMetroPI       "), MetroPI.UseMetroPI);
        inimoni::inirw(r, f, s, _T("MasConKey        "), MetroPI.MasConKey);
        inimoni::inirw(r, f, s, _T("Empty            "), MetroPI.Empty);
        inimoni::inirw(r, f, s, _T("Metro            "), MetroPI.Metro);
        inimoni::inirw(r, f, s, _T("Tobu             "), MetroPI.Tobu);
        inimoni::inirw(r, f, s, _T("Tokyu            "), MetroPI.Tokyu);
        inimoni::inirw(r, f, s, _T("Seibu            "), MetroPI.Seibu);
        inimoni::inirw(r, f, s, _T("Sotetsu          "), MetroPI.Sotetsu);
        inimoni::inirw(r, f, s, _T("JR               "), MetroPI.JR);
        inimoni::inirw(r, f, s, _T("Odakyu           "), MetroPI.Odakyu);
        inimoni::inirw(r, f, s, _T("Toyo             "), MetroPI.Toyo);

        s = _T("Emulate");
        inimoni::inirw(r, f, s, _T("Pitch            "), Emulate.Pitch);

        s = _T("Key");
        inimoni::inirw(r, f, s, _T("Switch           "), Key.Switch);
        return true;
    }

    void init()
    {
        Index.Blinking = 1;
        Index.Lighting = 13;
        Index.Buzzer = 13;
        Index.SW = 20;
        SN.UseSN = 1;
        MetroPI.UseMetroPI = 0;
        MetroPI.MasConKey = 92;
        MetroPI.Empty = 0;
        MetroPI.Metro = 0;
        MetroPI.Tobu = 1;
        MetroPI.Tokyu = 0;
        MetroPI.Seibu = 1;
        MetroPI.Sotetsu = 1;
        MetroPI.JR = 1;
        MetroPI.Odakyu = 1;
        MetroPI.Toyo = 0;
        Emulate.Pitch = 375;
        Key.Switch = _T("A2");
    }
};
typedef EBIni EBFile; //新旧互換

//---------------------------------------------------------------------------
// Common method                                                             
//---------------------------------------------------------------------------
#ifndef INIMONI_INIRW                                                        
#define INIMONI_INIRW                                                        
namespace inimoni
{
    /*
    Read and Write INI file
      int     is_read  1=Read mode, 0=Write mode
      string  fname    Filename (The Windows folder when there is not path)
      string  sec      Section name
      string  key      Key name
      T       val_t    [Read]Init+Output, [Write]Input
    */
    template<class T>
    bool inirw(int is_read, string_t& fname, string_t sec, string_t key, T& val_t)
    {
        if (is_read) {
            inimoni::read(fname.c_str(), sec.c_str(), key.c_str(), val_t);
        }
        else {
            inimoni::write(fname.c_str(), sec.c_str(), key.c_str(), val_t);
        }
        return true;
    }

    bool read(string_t ifn, string_t sec, string_t key, int& dst)
    {
        dst = GetPrivateProfileInt(sec.c_str(), key.c_str(), dst, ifn.c_str());
        return true;
    }

    bool read(string_t ifn, string_t sec, string_t key, basic_string<TCHAR>& dst)
    {
        TCHAR buf[256];
        GetPrivateProfileString(
            sec.c_str(),
            key.c_str(),
            dst.c_str(),
            buf,
            sizeof(buf),
            ifn.c_str());
        dst = buf;
        return true;
    }

    bool read(string_t ifn, string_t sec, string_t key, double& dst)
    {
        string_t s;
        inimoni::read(ifn, sec, key, s);

        TCHAR* e;
        double x = _tcstod(s.c_str(), &e);

        dst = 0.0;
        if (!*e) {
            dst = x;
        }
        return true;
    }

    template<class T>
    bool write(string_t ifn, string_t sec, string_t key, T val_t)
    {
        TCHAR val[1024];
        inimoni::to_string(val, val_t);
        WritePrivateProfileString(sec.c_str(), key.c_str(), val, ifn.c_str());
        return true;
    }

    void to_string(TCHAR* str, int val)
    {
        _stprintf(str, _T("%d"), val);
    }

    void to_string(TCHAR* str, double val)
    {
        _stprintf(str, _T("%f"), val);
    }

    void to_string(TCHAR* str, basic_string<TCHAR> val)
    {
        _stprintf(str, _T("%s"), val.c_str());
    }
};

#endif                                                                  
#endif                                                                  

