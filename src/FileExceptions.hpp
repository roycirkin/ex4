namespace FileExceptions{

class FileExceptions {
public:
    /**
     * @brief The function prints appopriate error message if something failes in the 
     * reading/writing from/to the file
     * 
     */
    virtual void printException();

};

class OpenFileExceptionForReading : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if file is in a bad state.
     * 
     */
    virtual void printException();
};

class ReadFileException : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if error occurred while reading the file.
     * 
     */
    virtual void printException();
};

class OpenFileExceptionForWriting : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if file is in a bad state.
     * 
     */
    virtual void printException();
};

class WriteFileException : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if error occurred while writing to file.
     * 
     */
    virtual void printException();
};

class FileNotBmpException : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if error occurred while writing to file.
     * 
     */
    virtual void printException();
};
class NULLPointerException : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if error occurred while writing to file.
     * 
     */
    virtual void printException();
};
class WrongBMPFileException : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if error occurred while writing to file.
     * 
     */
    virtual void printException();
};
class WrongMatrixFileException : public FileExceptions{
public:
    /**
     * @brief The function prints appopriate error message 
     * if error occurred while writing to file.
     * 
     */
    virtual void printException();
};

}