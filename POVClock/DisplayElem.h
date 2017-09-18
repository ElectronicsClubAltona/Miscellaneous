#ifndef DISPLAY_ELEM_H
#define DISPLAY_ELEM_H

#include <Arduino.h>




#define MAX_LINE 40
#define MAX_CHAR 40




class CClock;




class CPoint
{
    public:
    
        // Construction & destruction
        CPoint();
        CPoint(const uint8_t nStart, const uint16_t nAngle); 
        virtual ~CPoint();
        
        // Attributes
        virtual void doDisplay(CClock *pClock, const uint16_t nAngleRotation);
        void set(const uint8_t nStart, const uint16_t nAngle = 0);
        inline virtual void moveAround(const uint16_t nAngle)
        {
            m_nAngle += nAngle;
        };
        uint8_t getAngle()
        {
            return m_nAngle;
        };
        uint8_t getStart()
        {
            return m_nStart;
        };

    protected:
      
        // Data
        uint8_t m_nStart;
        uint16_t m_nAngle;
};
       



class CLine: public CPoint
{
    public:
    
        // Construction & destruction
        CLine();
        inline CLine(const uint8_t nStart, const uint8_t nEnd, const uint16_t nAngle = 0)
        {
            set(nStart, nEnd, nAngle);
        };
    
        inline ~CLine()
        {
        };
        
        // Attributes
        virtual void doDisplay(CClock *pClock, const uint16_t nAngleRotation);
        void set(const uint8_t nStart, const uint8_t nEnd, const uint16_t nAngle = 0);
        inline virtual void moveAround(const uint16_t nAngle)
        {
            CPoint::moveAround(nAngle);
        };

    protected:
      
        // Data
	      uint8_t m_nLength;
};
       



class CCharacter
{
    public:
    
        // Construction & destruction
        CCharacter();

        CCharacter(char cCh);

        ~CCharacter();
        
        // Attributes
        void doDisplay(CClock *pClock, float fAngleRotation);
        
        void moveAround(const uint16_t nAngle);
        
        operator char() const
        {
            return m_cCh;
        };

        void centre12oClock();
        unsigned getAngleWidth()
        {
            return m_nAngleWidth;
        };

        void addUnderscore();
        void removeUnderscore();
        
    protected:
      
        // Utility functions
        void addRadialLine(const uint16_t nAngle, const uint8_t nStart, const uint8_t nEnd);
        void addArc(const uint16_t nStartAngle, const uint16_t nEndAngle, const uint8_t nStart);
        void addPoint(const uint16_t nAngle, const uint8_t nStart);
        void doConstructCh();
        void removeElement(int8_t nI);
        
        // Data      
        CPoint* m_lineCollection[MAX_LINE];
        
        uint8_t m_nSize;
        char m_cCh;
        unsigned m_nAngleWidth, m_nAngleStart;
        static unsigned m_nAngleInc;
        static uint8_t m_nCharTop, m_nCharBottom, m_nCharMiddle, m_nCharHeight;
};




const byte nMaxBuff = 40;

class CText
{
    public:
    
        // Construction & destruction
        CText();

        CText(const __FlashStringHelper *strfText)
        {
            CText();
            *this = strfText;
        }
        
        CText(const char* lpszText)
        {
            CText();
            *this = lpszText;
        };

        CText(const uint8_t nNum, const int bPad = false);
        
        inline ~CText()
        {
            empty();
        };
        
        // Attributes
        inline void doDisplay(CClock *pClock, float fAngle)
        {
            for (int8_t nI = 0; nI < MAX_CHAR; nI++)
                m_characterCollection[nI]->doDisplay(pClock, fAngle);
        };
        
        void addCharacter(char cChar, bool bToFront = false);
        CText& operator =(const __FlashStringHelper *strfText);
        CText& operator =(const char* lpszText);
        CText& operator =(const int8_t nNum);
        CText& operator =(const uint8_t nNum);
        
        void operator +=(const __FlashStringHelper *lpszText);  
        void operator +=(const char* lpszText);
        void operator +=(const uint8_t nNum);
        
        CText& operator +(const __FlashStringHelper *lpszText)
        {
            return *this + (char*)lpszText;
        };

        CText& operator +(const char* lpszText);
        CText& operator +(const uint8_t nNum);
        
        friend CText& operator +(const __FlashStringHelper *lpszText, const CText& strText)
        {
            return (const char*)lpszText + strText;
        };
        
        friend CText& operator +(const char* lpszText, const CText& strText);
        friend CText& operator +(const uint8_t nNum, const CText& strText);
        void empty();
        operator const char*()const;
        int8_t length()
        {
            return m_nLength;
        };
        
        static char m_strBuff[nMaxBuff];

    protected:
      
        // Utility functions
        float getAngleWidth();
        void centreAt12oClock();
        void doPadLeft(const char cChar);

        // Data      
        CCharacter* m_characterCollection[MAX_CHAR];
        uint8_t m_nLength;
        static uint8_t m_nAngleCharSpacing;
        bool m_bHasUnderscore;
};

#endif









