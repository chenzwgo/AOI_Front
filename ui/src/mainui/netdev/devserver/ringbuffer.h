#ifndef __RingBuffer_h
#define __RingBuffer_h
#include <string>
using namespace std;

//  如多线进行读写时,需要进行互斥操作
class CRingBuffer  
{
protected:

	char * m_pBuf;
	char * m_pTmpBuf; // dupe buf for working so no alloc/free while running.
	int m_nBufSize;   // the size of the ring buffer
	int m_iReadPtr;   // the read pointer
	int m_iWritePtr;  // the write pointer
	int m_iWriteSize;
public:



	CRingBuffer(int iBufSize)
	{
		m_pBuf = NULL;
		m_pTmpBuf = NULL;
		m_nBufSize = 0;
		m_iReadPtr = 0;
		m_iWritePtr = 0;
		m_iWriteSize = iBufSize;
		Create(iBufSize);
	}

	virtual ~CRingBuffer()
	{
		Destroy();
	}

	///////////////////////////////////////////////////////////////////
	// Method: PushBinary
	// Purpose: Writes binary data into the ring buffer.
	// Parameters:
	//     [in] pBuf - Pointer to the data to write.
	//     [in] nBufLen - Size of the data to write (in bytes).
	// Return Value: TRUE upon success, otherwise FALSE.
	// 
	int PushBinary( char * pBuf, int nBufLen )
	{
		int iResult = 0;
		{
			if (nBufLen >= GetMaxWriteSize())
			{
				ReMalloc(m_nBufSize + nBufLen);
			}
			if( nBufLen <= GetMaxWriteSize() )
			{
				// easy case, no wrapping
				if( m_iWritePtr + nBufLen < m_nBufSize )
				{
					memcpy(&m_pBuf[m_iWritePtr], pBuf, nBufLen);
					m_iWritePtr += nBufLen;		
				}
				else // harder case we need to wrap
				{
					int iFirstChunkSize = m_nBufSize - m_iWritePtr;
					int iSecondChunkSize = nBufLen - iFirstChunkSize;

					memcpy( &m_pBuf[m_iWritePtr], pBuf, iFirstChunkSize );
					memcpy( &m_pBuf[0], &pBuf[iFirstChunkSize], iSecondChunkSize );

					m_iWritePtr = iSecondChunkSize;
				}
				iResult = nBufLen;
				m_iWriteSize = m_iWriteSize - nBufLen;
			}	
		}
		return iResult;
	}
	///////////////////////////////////////////////////////////////////
	// Method: PopBinary
	// Purpose: Reads (and extracts) data from the ring buffer.
	// Parameters:
	//     [in/out] pBuf - Pointer to where read data will be stored.
	//     [in] nBufLen - Size of the data to be read (in bytes).
	// Return Value: TRUE upon success, otherwise FALSE.
	// 
	int PopBinary( char * pBuf, int nBufLen )
	{
		int iResult = 0;
		{
			if (nBufLen > GetMaxReadSize())
			{
				nBufLen = GetMaxReadSize();
			}
			if (nBufLen == 0)
			{
				return 0;
			}
			if( nBufLen <= GetMaxReadSize() )
			{
				// easy case, no wrapping
				if( m_iReadPtr + nBufLen <= m_nBufSize )
				{
					memcpy( pBuf, &m_pBuf[m_iReadPtr], nBufLen );
					memset(&m_pBuf[m_iReadPtr],0, nBufLen );
					m_iReadPtr += nBufLen;			
				}
				else // harder case, buffer wraps
				{
					int iFirstChunkSize = m_nBufSize - m_iReadPtr;
					int iSecondChunkSize = nBufLen - iFirstChunkSize;

					memcpy( pBuf, &m_pBuf[m_iReadPtr], iFirstChunkSize );
					memset(&m_pBuf[m_iReadPtr],0, iFirstChunkSize );

					memcpy(&pBuf[iFirstChunkSize], &m_pBuf[0], iSecondChunkSize );
					memset(&m_pBuf[0],0, iSecondChunkSize );

					m_iReadPtr = iSecondChunkSize;
				}
				iResult =nBufLen;
				m_iWriteSize = m_iWriteSize + nBufLen;
			}	
		}
		return iResult;
	}

	void ClearBuffer()
	{
		if( m_pBuf )
			memset(m_pBuf,0,m_nBufSize);

		if( m_pTmpBuf )
			memset(m_pTmpBuf,0,m_nBufSize);

		m_iReadPtr = 0;
		m_iWritePtr = 0;
		m_iWriteSize = m_nBufSize;
	}

	///////////////////////////////////////////////////////////////////
	// Method: PeekChar
	// Purpose: Peeks at a character at the given position in the ring
	//     buffer, without extracting it.
	// Parameters:
	//     [in] iPos - Index of the character to peek (zero-based).
	//     [out] ch - The character peeked.
	// Return Value: TRUE upon success, otherwise FALSE.
	// 
	bool PeekChar( int iPos, char & ch )
	{
		bool bResult = false;
		{
			if( iPos < GetMaxReadSize() )
			{
				if( m_iWritePtr > m_iReadPtr )
				{
					// easy case, buffer doesn't wrap
					ch = m_pBuf[ m_iReadPtr+iPos ];
					bResult = true;
				}
				else if( m_iWritePtr == m_iReadPtr )
				{
					// nothing in the buffer
				}
				else if( m_iWritePtr < m_iReadPtr )
				{
					// harder case, buffer wraps

					if( m_iReadPtr + iPos < m_nBufSize )
					{
						// pos was in first chunk
						ch = m_pBuf[ m_iReadPtr + iPos ];
						bResult = true;
					}
					else
					{
						// pos is in second chunk
						ch = m_pBuf[ iPos - (m_nBufSize-m_iReadPtr) ];
						bResult = true;
					}
				}
			}
		}
		return bResult;
	}


	///////////////////////////////////////////////////////////////////
	// Method: GetMaxReadSize
	// Purpose: Returns the amount of data (in bytes) available for
	//     reading from the buffer.
	// Parameters: (None)
	// Return Value: Amount of data (in bytes) available for reading.
	//
	int GetMaxReadSize()
	{
		if( m_pBuf )
		{
			return m_nBufSize - m_iWriteSize;
			if( m_iReadPtr == m_iWritePtr )
				return 0;

			if( m_iReadPtr < m_iWritePtr )
				return m_iWritePtr - m_iReadPtr;

			if( m_iReadPtr > m_iWritePtr )
				return (m_nBufSize-m_iReadPtr)+m_iWritePtr;
		}
		return 0;
	}

	///////////////////////////////////////////////////////////////////
	// Method: GetMaxWriteSize
	// Purpose: Returns the amount of space (in bytes) available for
	//     writing into the buffer.
	// Parameters: (None)
	// Return Value: Amount of space (in bytes) available for writing.
	//
	int GetMaxWriteSize()
	{
		if( m_pBuf )
		{
			return m_iWriteSize;
			if( m_iReadPtr == m_iWritePtr )
				return m_nBufSize;

			if( m_iWritePtr < m_iReadPtr )
				return m_iReadPtr - m_iWritePtr;

			if( m_iWritePtr > m_iReadPtr )
				return (m_nBufSize-m_iWritePtr)+m_iReadPtr;
		}
		return 0;
	}



	///////////////////////////////////////////////////////////////////
	// Method: FindChar
	// Purpose: Determines if the specified character is in the ring
	//     buffer, and if so, returns the index position.
	// Parameters:
	//     [in] chLookFor - Character to look for in the ring buffer.
	//     [out] riPos - The index position of the character, if found.
	// Return Value: TRUE upon success, otherwise FALSE.
	// 
	bool FindChar( char chLookFor, int & riPos )
	{
		bool bResult = false;
		{
			int iSize = GetMaxReadSize();

			for( int i = 0; i < iSize; i++ )
			{
				char ch;
				if( PeekChar( i, ch ) )
				{
					if( ch == chLookFor )
					{
						riPos = i;
						bResult = true;
						break;
					}
				}
			}
		}
		return bResult;
	}

	bool FindChars(const char* chLookFor, int & riPos )
	{

			bool bfound = true;
			int iSize = GetMaxReadSize();
			int iCharlength = strlen(chLookFor);
			if (iSize < iCharlength)
			{
				return false;
			}
			for( int i = 0; i < iSize; i++ )
			{
				for (int index = 0 ;index < iCharlength; index++)
				{
					char ch;
					if( PeekChar( i+index, ch ) )
					{
						if ( chLookFor[index] == ch )
						{
							if (index == iCharlength-1)
							{
 								riPos = i + index;
								return true;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		    return false;
	}


	///////////////////////////////////////////////////////////////////
	// Method: ReadTextLine
	// Purpose: Reads a line of text from the buffer, if available.
	// Parameters:
	//     [out] strLine - The line of text.
	// Return Value: TRUE upon success, otherwise FALSE.
	// 
	bool ReadTextLine( string & strLine )
	{
		bool bResult = false;
		{
			int iPos = 0;
			if( FindChar('\n',iPos) )
			{
				int iSize = iPos+1;
				memset(m_pTmpBuf,0,m_nBufSize);
				if( PopBinary( m_pTmpBuf, iSize ) )
				{
					m_pTmpBuf[ iSize ] = '\0';
					strLine = m_pTmpBuf;
					bResult = true;
				}
			}
		}
		return bResult;
	}

	bool ReadTextByMask( string & strLine ,string strMask)
	{
		bool bResult = false;
		{
			int iPos = 0;
			if( FindChars(strMask.c_str(),iPos) )
			{
				int iSize = iPos+1;
				memset(m_pTmpBuf,0,m_nBufSize);
				if( PopBinary( m_pTmpBuf, iSize ) )
				{
					m_pTmpBuf[ iSize ] = '\0';
					strLine = m_pTmpBuf;
					bResult = true;
				}
			}
		}
		return bResult;
	}

private:

	///////////////////////////////////////////////////////////////////
	// Method: Create
	// Purpose: Initializes the ring buffer for use.
	// Parameters:
	//     [in] iBufSize -- maximum size of the ring buffer
	// Return Value: TRUE if successful, otherwise FALSE.
	//
	bool Create( int iBufSize )
	{
		if (iBufSize < 0 )
		{
			return false;
		}
		bool bResult = false;
		{
			m_pBuf = new char[ iBufSize ];
			if( m_pBuf )
			{
				m_nBufSize = iBufSize;
				memset(m_pBuf,0,m_nBufSize);
		
				m_pTmpBuf = new char[ iBufSize ];
				if( m_pTmpBuf )
				{
					memset(m_pTmpBuf,0,m_nBufSize);
                    bResult = true;
				}
			}
		}
		return bResult;
	}

	///////////////////////////////////////////////////////////////////
	// Method: Destroy
	// Purpose: Cleans up ring buffer by freeing memory and resetting
	//     member variables to original state.
	// Parameters: (None)
	// Return Value: (None)
	//
	void Destroy()
	{
		if( m_pBuf )
			delete m_pBuf;

		if( m_pTmpBuf )
			delete m_pTmpBuf;

		m_pBuf = NULL;
		m_pTmpBuf = NULL;
		m_nBufSize = 0;
		m_iReadPtr = 0;
		m_iWritePtr = 0;
	}


	bool ReMalloc(int iSize)
	{
		if (iSize <= m_nBufSize)
		{
			return true;
		}

		bool bResult = false;
		{
			char* m_pNewBuf = new char[iSize];
			if( m_pNewBuf && m_pBuf)
			{	
				memset(m_pNewBuf,0,iSize);
				memcpy(m_pNewBuf,m_pBuf,m_nBufSize);
				m_iWriteSize = m_iWriteSize+ iSize - m_nBufSize;
				m_nBufSize = iSize;

				delete m_pBuf;
				m_pBuf = m_pNewBuf;
		
				if (m_pTmpBuf)
				{
					delete m_pTmpBuf;
					m_pTmpBuf = NULL;
				}
				m_pTmpBuf = new char[ iSize ];
				if( m_pTmpBuf )
				{
					memset(m_pTmpBuf,0,m_nBufSize);
                    bResult = true;
				}
			}
		}
		return bResult;	
	}
};

#endif//__RingBuffer_h

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
