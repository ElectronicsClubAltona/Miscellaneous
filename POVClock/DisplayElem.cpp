#include "Clock.h"
#include "DisplayElem.h"
#include "debug.h"

  


CPoint::CPoint(const uint8_t nStart, const uint16_t nAngle)
{
	set(nStart, nAngle);
}
            
CPoint::~CPoint()
{
}

CPoint::CPoint()
{
    m_nStart = 0;
    m_nAngle = 0;
}
    
void CPoint::doDisplay(CClock *pClock, const uint16_t nAngleRotation)
{
    if (nAngleRotation == m_nAngle)
        pClock->line(m_nStart, 1, pClock->getTextColor());
}

void CPoint::set(const uint8_t nStart, const uint16_t nAngle)
{
    if ((nStart >= 0) && (nStart < NUM_LEDS))
    {
        m_nStart = nStart;
        m_nAngle = nAngle;
    }
}








CLine::CLine()
{
    m_nLength = 0;
}
    
void CLine::doDisplay(CClock *pClock, const uint16_t nAngleRotation)
{
    if (nAngleRotation == m_nAngle)
        pClock->line(m_nStart, m_nLength, pClock->getTextColor());
}

void CLine::set(const uint8_t nStart, const uint8_t nEnd, const uint16_t nAngle)
{
    if ((nStart >= 0) && (nStart < NUM_LEDS) && (nEnd >= 0) && (nEnd < NUM_LEDS) && (nEnd > nStart))
    {
        CLine::set(nStart, nAngle);
        m_nLength = nEnd - nStart + 1;
    }
}








uint8_t CCharacter::m_nCharTop = NUM_LEDS;

uint8_t CCharacter::m_nCharHeight = 7;

uint8_t CCharacter::m_nCharBottom = NUM_LEDS - m_nCharHeight + 1;

uint8_t CCharacter::m_nCharMiddle = NUM_LEDS - (m_nCharHeight / 2);

unsigned CCharacter::m_nAngleInc = 6;

CCharacter::CCharacter(char cCh)
{
    m_nSize = 0;
    m_nAngleWidth = 30;
    m_nAngleStart = 0;
    m_cCh = cCh;
    memset(m_lineCollection, 0, MAX_LINE);
    doConstructCh();
}

CCharacter::CCharacter()
{
    m_nSize = 0;
    m_nAngleWidth = 30;
    m_cCh = 0;
    memset(m_lineCollection, 0, MAX_LINE);
}

CCharacter::~CCharacter()
{
    for (uint8_t nI = 0; nI < m_nSize; nI++)
        delete m_lineCollection[nI];
    memset(m_lineCollection, 0, MAX_LINE);
}

void CCharacter::doDisplay(CClock *pClock, float fAngleRotation)
{
    for (uint8_t nI = 0; nI <= m_nSize; nI++)
        m_lineCollection[nI]->doDisplay(pClock, fAngleRotation);
}

void CCharacter::moveAround(const uint16_t nAngle)
{
    for (uint8_t nI = 0; nI <= m_nSize; nI++)
    {
        if (m_lineCollection[nI])
            m_lineCollection[nI]->moveAround(nAngle);
    }
    m_nAngleStart += nAngle;
}

void CCharacter::addRadialLine(const uint16_t nAngle, const uint8_t nStart, const uint8_t nEnd)
{
    if (m_nSize < MAX_LINE)
    {
        m_lineCollection[m_nSize] = new CLine(nStart, nEnd, nAngle);
        m_nSize++;
    }
}

void CCharacter::addArc(const uint16_t nStartAngle, const uint16_t nEndAngle, const uint8_t nStart)
{
    if (m_nSize < MAX_LINE)
    {
        unsigned nI = nStartAngle;
        while (nI <= nEndAngle) 
        {
            m_lineCollection[m_nSize] = new CPoint(nStart, nI);
            m_nSize++;
            nI += m_nAngleInc;
        }
    }
}

void CCharacter::addPoint(const uint16_t nAngle, const uint8_t nStart)
{
  if (m_nSize < MAX_LINE)
  {
      m_lineCollection[m_nSize] = new CPoint(nStart, nAngle);
      m_nSize++;
  }
}

void CCharacter::addUnderscore()
{
    addArc(m_nAngleStart, m_nAngleStart + m_nAngleWidth, m_nCharBottom - 2);
}

void CCharacter::removeElement(int8_t nI)
{
    if ((nI >= 0) && (nI < m_nSize))
    {
        delete m_lineCollection[nI];
        for (nI = nI; nI < (m_nSize - 1); nI++)
            m_lineCollection[nI] = m_lineCollection[nI + 1];
        m_nSize--;
    }
}

void CCharacter::removeUnderscore()
{
    uint8_t nI = 0;
    
    while (nI < m_nSize)
    {
        if (m_lineCollection[nI]->getStart() < m_nCharBottom)
            removeElement(nI);
        else
            nI++;
    }
}

void CCharacter::doConstructCh()
{
    switch (m_cCh)
    {
        case ':':
              addPoint(m_nAngleWidth / 2, m_nCharTop - 2);
              addPoint(m_nAngleWidth / 2, m_nCharBottom + 2);
              break;
        case '-':
              addArc(m_nAngleInc * 2, m_nAngleWidth - (m_nAngleInc * 2), m_nCharMiddle);
              break;
        case 'a':
        case 'A': 
              addRadialLine(0, m_nCharBottom, m_nCharTop - 3);
              addRadialLine(m_nAngleWidth, m_nCharBottom, m_nCharTop - 3);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              break;
        case 'b':
        case 'B': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addRadialLine(m_nAngleWidth, m_nCharTop - 2, m_nCharTop - 1);
              addRadialLine(m_nAngleWidth, m_nCharTop - 5, m_nCharTop - 4);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              break;
        case 'c':
        case 'C': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addPoint(m_nAngleWidth, m_nCharTop - 1);
              addPoint(0, m_nCharBottom + 1);
              break;
        case 'd':
        case 'D': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop - 1);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              break;
        case 'e' :   
        case 'E': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharBottom);
              addArc(m_nAngleInc, m_nAngleWidth - (2 * m_nAngleInc), m_nCharMiddle);
              break;
        case 'f':
        case 'F': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - (2 * m_nAngleInc), m_nCharMiddle);
              break;
        case 'g': 
        case 'G': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addPoint(m_nAngleWidth, m_nCharTop - 1);
              addRadialLine(m_nAngleWidth, m_nCharBottom, m_nCharMiddle - 1);
              addRadialLine(m_nAngleWidth - (2 * m_nAngleInc), m_nAngleWidth - m_nAngleInc, m_nCharMiddle - 1);
              break;
        case 'h':
        case 'H': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addRadialLine(m_nAngleWidth, m_nCharBottom, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              break;
        case 'i':
        case 'I': 
              m_nAngleWidth /= 2;
              addRadialLine(m_nAngleWidth / 2, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(0, m_nAngleWidth, m_nCharTop);
              addArc(0, m_nAngleWidth, m_nCharBottom);
              break;
        case 'j':
        case 'J': 
              addRadialLine(m_nAngleWidth - m_nAngleInc, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(m_nAngleWidth - (2 * m_nAngleInc), m_nAngleWidth, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - (2 * m_nAngleInc), m_nCharBottom);
              addPoint(0, m_nCharBottom + 1);
              break;
        case 'k':
        case 'K': 
              m_nAngleWidth -= m_nAngleInc;
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addPoint(m_nAngleInc, m_nCharMiddle);
              addPoint(m_nAngleInc * 2, m_nCharMiddle + 1);
              addPoint(m_nAngleInc * 2, m_nCharMiddle - 1);
              addPoint(m_nAngleInc * 3, m_nCharMiddle + 2);
              addPoint(m_nAngleInc * 3, m_nCharMiddle - 2);
              addPoint(m_nAngleInc * 4, m_nCharMiddle + 3);
              addPoint(m_nAngleInc * 4, m_nCharMiddle - 3);
              break;
        case 'l':
        case 'L': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharBottom);
              break;
        case 'm':
        case 'M': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addRadialLine(m_nAngleWidth, m_nCharBottom, m_nCharTop);
              addPoint(m_nAngleInc, m_nCharTop - 1);
              addPoint(2 * m_nAngleInc, m_nCharBottom - 2);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleWidth - (2 * m_nAngleInc), m_nCharTop - 2);
              addPoint(m_nAngleWidth / 2, m_nCharBottom - 3);
              break;
        case 'n':
        case 'N': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addRadialLine(m_nAngleWidth, m_nCharBottom, m_nCharTop);
              addPoint(m_nAngleInc, m_nCharTop - 1);
              addPoint(2 * m_nAngleInc, m_nCharBottom - 2);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharBottom + 1);
              addPoint(m_nAngleWidth - (2 * m_nAngleInc), m_nCharBottom + 2);
              addPoint(m_nAngleWidth / 2, m_nCharMiddle);
              break;
        case 'o':
        case 'O': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharTop - 1);
              break;
        case 'p':
        case 'P': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharMiddle);
              addRadialLine(m_nAngleWidth, m_nCharMiddle + 1, m_nCharTop - 1);
              break;
        case 'q':
        case 'Q': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharTop - 1);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharBottom + 1);
              break;
        case 'r':
        case 'R': 
              addRadialLine(0, m_nCharBottom, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth, m_nCharMiddle);
              addRadialLine(m_nAngleWidth, m_nCharMiddle + 1, m_nCharTop - 1);
              addPoint(m_nAngleWidth - (2 * m_nAngleInc), m_nCharBottom + 2);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharBottom + 1);
              addPoint(m_nAngleWidth, m_nCharBottom);
              break;
        case 's':
        case 'S': 
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharMiddle - 1);
              addPoint(m_nAngleWidth, m_nCharTop - 1);
              addRadialLine(0, m_nCharMiddle + 1, m_nCharTop - 1);
              addPoint(0, m_nCharBottom + 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              break;
        case 't':
        case 'T': 
              addRadialLine(m_nAngleWidth / 2, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(0, m_nAngleWidth, m_nCharTop);
              break;
        case 'u':
        case 'U': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharTop);
              break;
        case 'v':
        case 'V': 
              addRadialLine(0, m_nCharMiddle + 1, m_nCharTop);
              addRadialLine(m_nAngleWidth, m_nCharMiddle + 1, m_nCharTop);
              addPoint(m_nAngleInc, m_nCharBottom + 2);
              addPoint(m_nAngleInc * 2, m_nCharBottom + 1);
              addPoint(m_nAngleWidth / 2, m_nCharBottom);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharBottom + 1);
              addPoint(m_nAngleWidth - (m_nAngleInc * 2), m_nCharBottom + 2);
              break;
        case 'w':
        case 'W': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharTop);
              addPoint(m_nAngleInc, m_nCharBottom);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addPoint(m_nAngleInc * 2, m_nCharBottom - 1);
              addPoint(m_nAngleWidth - (2 * m_nAngleInc), m_nCharBottom - 1); 
              addPoint(m_nAngleWidth / 2, m_nCharBottom - 2);
              break;
        case 'x':
        case 'X': 
              addPoint(0, m_nCharTop);
              addPoint(0, m_nCharBottom);
              addPoint(m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleInc, m_nCharBottom + 1);
              addPoint(2 * m_nAngleInc, m_nCharTop - 2);
              addPoint(2 * m_nAngleInc, m_nCharBottom + 2);
              addPoint(0, m_nCharBottom);
              addPoint(m_nAngleWidth / 2, m_nCharMiddle);
              addPoint(m_nAngleWidth, m_nCharTop);
              addPoint(m_nAngleWidth, m_nCharBottom);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharBottom + 1);
              addPoint(m_nAngleWidth - (2 * m_nAngleInc), m_nCharTop - 2);
              addPoint(m_nAngleWidth - (2 * m_nAngleInc), m_nCharBottom + 2);
              break;
        case 'y':
        case 'Y': 
              addRadialLine(0, m_nCharBottom, m_nCharMiddle);
              addPoint(0, m_nCharTop);
              addPoint(m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleInc * 2, m_nCharTop - 2);
              addPoint(m_nAngleWidth, m_nCharTop);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleWidth - (m_nAngleInc * 2), m_nCharTop - 2);
              break;
        case 'z':
        case 'Z': 
              addArc(0, m_nAngleWidth, m_nCharTop);
              addArc(0, m_nAngleWidth, m_nCharBottom);
              addPoint(m_nAngleInc, m_nCharBottom + 1);
              addPoint(m_nAngleInc * 2, m_nCharMiddle + 2);
              addPoint(m_nAngleWidth / 2, m_nCharMiddle);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleWidth - (m_nAngleInc * 2), m_nCharTop - 2);
              break;
        case '0': 
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop - 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharTop - 1);
              addPoint(m_nAngleInc, m_nCharBottom + 1);
              addPoint(m_nAngleInc * 2, m_nCharMiddle + 2);
              addPoint(m_nAngleWidth / 2, m_nCharMiddle);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleWidth - (m_nAngleInc * 2), m_nCharTop - 2);
              break;
        case '1': 
              m_nAngleWidth /= 2;
              addRadialLine(m_nAngleWidth / 2, m_nCharBottom + 1, m_nCharTop - 1);
              addPoint(m_nAngleInc * 2, m_nCharTop - 1);
              addPoint(m_nAngleInc * 1, m_nCharTop - 2);
              addArc(0, m_nAngleWidth, m_nCharBottom);
              break;
        case '2': 
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(0, m_nAngleWidth, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharTop - 2, m_nCharTop - 1);
              addPoint(4 * m_nAngleInc, m_nCharMiddle);
              addPoint(3 * m_nAngleInc, m_nCharMiddle - 1);
              addPoint(2 * m_nAngleInc, m_nCharMiddle - 2);
              addPoint(m_nAngleInc, m_nCharMiddle - 2);
              break;
        case '3': 
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharMiddle - 1);
              addRadialLine(m_nAngleWidth, m_nCharMiddle + 1,  m_nCharTop - 1);
              addPoint(0, m_nCharTop - 1);
              addPoint(0, m_nCharBottom + 1);
              break;
        case '4': 
              addArc(0, m_nAngleWidth, m_nCharBottom + 2);
              addRadialLine(0, m_nCharBottom + 1, m_nCharTop);
              addRadialLine(m_nAngleWidth - (2 * m_nAngleInc), m_nCharBottom, m_nCharMiddle - 1);
              break;
        case '5': 
              addArc(0, m_nAngleWidth, m_nCharTop);
              addRadialLine(0, m_nCharMiddle, m_nCharTop - 1);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth ,m_nCharBottom + 1, m_nCharMiddle - 1);
              break;
        case '6': 
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharMiddle - 1);
              addPoint(m_nAngleWidth, m_nCharTop - 1);
              break;
        case '7': 
              addArc(0, m_nAngleWidth, m_nCharTop);
              addPoint(m_nAngleInc, m_nCharBottom + 1);
              addPoint(m_nAngleInc * 2, m_nCharMiddle + 2);
              addPoint(m_nAngleWidth / 2, m_nCharMiddle);
              addPoint(m_nAngleWidth - m_nAngleInc, m_nCharTop - 1);
              addPoint(m_nAngleWidth - (m_nAngleInc * 2), m_nCharTop - 2);
              break;
        case '8': 
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addRadialLine(m_nAngleWidth, m_nCharBottom + 1, m_nCharMiddle - 1);
              addRadialLine(m_nAngleWidth, m_nCharMiddle + 1,  m_nCharTop - 1);
              addRadialLine(0, m_nCharBottom + 1, m_nCharMiddle - 1);
              addRadialLine(0, m_nCharMiddle + 1,  m_nCharTop - 1);
              break;
        case '9': 
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharTop);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharMiddle);
              addArc(m_nAngleInc, m_nAngleWidth - m_nAngleInc, m_nCharBottom);
              addPoint(m_nAngleWidth, m_nCharBottom + 1);
              addRadialLine(m_nAngleWidth, m_nCharMiddle + 1,  m_nCharTop - 1);
              break;
    }
}







class CText;

CText *ptext = new CText;

uint8_t CText::m_nAngleCharSpacing = 6;

char CText::m_strBuff[MAX_CHAR];

CText::CText()
{
    m_nLength = 0;
    memset(m_characterCollection, MAX_CHAR, NULL);
    memset(m_strBuff, 0, MAX_CHAR);
    m_bHasUnderscore = false;
}

CText::CText(const uint8_t nNum, const int bPad)
{
    CText();
    *this = nNum;
    if (bPad)
        doPadLeft('0');
};

void CText::doPadLeft(const char cChar)
{
    while (length() < 2)
        addCharacter(cChar, true);
}

void CText::addCharacter(char cChar, bool bToFront)
{
    int8_t nI = 0;
    CCharacter* pChar = NULL;

    if ((m_nLength + 1) < MAX_CHAR)
    {
        pChar = new CCharacter(cChar);
        pChar->moveAround(getAngleWidth() + m_nAngleCharSpacing);
        
        if (!bToFront)
        {
            m_characterCollection[m_nLength++] = pChar;
        }
        else
        {
            for (nI = 0; nI < m_nLength; nI++)
            {
                m_characterCollection[nI + 1] = m_characterCollection[nI];
            }
            m_characterCollection[0] = pChar;
        }
    }
}

void CText::centreAt12oClock()
{
    float fAngleRotate = getAngleWidth() / 2;

    for (uint8_t nI = 0; nI < m_nLength; nI++)
        m_characterCollection[nI]->moveAround(360.0 - fAngleRotate);
}

float CText::getAngleWidth()
{
    float fAngleWidth = 0;
    
    for (uint8_t nI = 0; nI < m_nLength; nI++)
        fAngleWidth += m_characterCollection[nI]->getAngleWidth();

    return fAngleWidth;
}

CText::operator const char*()const
{
    memset(m_strBuff, 0, MAX_CHAR);
   
    for (uint8_t nI = 0; nI < m_nLength; nI++)
    {
        if (nI == MAX_CHAR)
        {
            debug.log("CText::m_strBuff overflow, DisplayElem.cpp, line 624...");
            break;
        }
        else
        {
            m_strBuff[nI] = *(m_characterCollection[nI]);
        }
    }
#ifdef DEBUG
    if (m_bHasUnderscore)
    {
        strcat(m_strBuff, "\r\n");
        for (uint8_t nI = 0; nI < m_nLength; nI++)
        {
            if (!m_characterCollection[nI]->isUnderscored())
            {
                strcat(m_strBuff, " ");
            }
            else
            {
                strcat(m_strBuff, "-");
            }
        }
    }
#endif
    return m_strBuff;

}

void CText::empty()
{
    for (uint8_t nI = 0; nI < m_nLength; nI++)
    {
        delete m_characterCollection[nI];
        m_characterCollection[nI] = NULL;
    }
    m_nLength = 0;
}

CText& CText::operator =(const char* lpszText)
{
    empty();
    for (int8_t nI = 0, nLen = strlen(lpszText); nI < nLen; nI++)
    {
        addCharacter(lpszText[nI]);
    }    
    centreAt12oClock();
    
    return *this;
}

CText& CText::operator =(const uint8_t nNum)
{
    char szNum[4] = {0, 0, 0, 0};
    
    sprintf(szNum, (const char*)F("%d"), nNum);
    *this = szNum;

    return *this;
}

CText& CText::operator =(const __FlashStringHelper *strfText)
{
    empty();

    PGM_P str = reinterpret_cast<PGM_P>(strfText);
    unsigned char cChar = 0;
    
    while (true) 
    {
        cChar = pgm_read_byte(str++);
        if (cChar == 0) 
            break;
        addCharacter(cChar);
    }
	return *this;
}

void CText::operator +=(const __FlashStringHelper *strfText)
{
    PGM_P str = reinterpret_cast<PGM_P>(strfText);
    unsigned char cChar = 0;
    
    while (true) 
    {
        cChar = pgm_read_byte(str++);
        if (cChar == 0) 
            break;
        addCharacter(cChar);
    }
}

void CText::operator +=(const char* lpszText)
{
    for (int8_t nI = 0, nLen = strlen(lpszText); nI < nLen; nI++)
        addCharacter(lpszText[nI]);
    centreAt12oClock();
}

void CText::operator +=(const uint8_t nNum)
{
    *ptext = nNum;
    *this += *ptext;
}

CText& CText::operator +(const char* lpszText)
{
    *ptext = *this;
    *ptext += lpszText;

    return *ptext;    
}

CText& CText::operator +(const uint8_t nNum)
{
    *ptext = nNum;
    *ptext += *ptext;

    return *ptext;
}

CText& operator +(const char* lpszText, const CText& strText)
{
    *ptext = lpszText;
    *ptext += strText;

    return *ptext;
}

CText& operator +(const uint8_t nNum, const CText& strText)
{
    *ptext = nNum;
    *ptext += strText;

    return *ptext;
}






