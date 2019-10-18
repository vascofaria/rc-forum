1. About rc-forum 

    Rc-forum is a simple online discussion forum about Redes de Computadores.
    It is composed by both Server and User Application.

2. Installation

    In order to compile rc-forum in your machine, make sure you have gcc compiler 
    installed:

        $ which gcc
        $ which make

    If not, and you're using Debian/Ububtu you can install it:  $ sudo apt install 
    build-essential 

3. Usage

    Compile rc-forum with the command "make" at the root of the project and then 
    you can run the Server and the User Application.

        Run server [in rc-forum/forum-server]:  ./FS [-p FSport]
            - FSport is the port where the server will accept UDP and TCP requests.
             The default port is 58001.

        Run user [in rc-forum/forum-user]: ./user [-n FSIP] [-p FSport]
            - FSIP is the server-name or IP adress.
            - FSport is the server's well known port where TCP and UDP requests are 
            accepted.

    With both applications running, the user application is ready to receive a command 
    and points out the valid options. 

        - ? - displays all valid commands.

        - register/reg	<userId> - makes the registry of the userID.

        - topic_list/tl - lists the available topics on the forum.

	    - topic_select <topic name> - selects a topic by name, only works if the topics 
                                    were listed before.

	    - ts <topic number> - selects a topic by number.

        - topic_propose	/tp	<topic> - adds a new topic if it's not a duplicate or if the
                                    maximum number of topics hasn't been reached yet. 
                                    When a topic is proposed it gets automatically selected. 

        - question_list	/ql - lists all the questions on a selected topic.

        - question_get <question name> - gives the 10 latest answers on a question with its' 
                                        images (if it has any associated) and/or  the image
                                        of the question itself it it has any.

        - qg <question_number> - has the same effect as question_get except has to have 
                                        a topic selected.
 
        - answer_submit	/as	<text_file [image_file]> - submits an answer to a question 
                                                    and gets server's feedback on the 
                                                    status of the submission. Assumes
                                                    that the text file's extension is txt
                                                    and the image's extension cannot be txt.
                                                    See section 6 - Directories Manipulation.

        - question_submit	/qs	<text_file [image_file]> - submits a question on a selected 
                                                        topic and receives the server's 
                                                        feedback on the submission. The
                                                        extension of the file is assumed to 
                                                        be txt and the question gets selected.
                                                        See section 6 - Directories Manipulation.


        - exit - closes the connection with the server.


4. Project Tree 
     Has an example of the directory's structure that are being use when files and directories are created.

        rc-forum
        |
        ├── forum-server
        │   ├── constants.h
        │   ├── entities
        │   │   ├── answer.c
        │   │   ├── answer.h
        │   │   ├── question.c
        │   │   └── question.h
        │   ├── exceptions.h
        │   ├── file-manager
        │   │   ├── answer-file-manager.c
        │   │   ├── answer-file-manager.h
        │   │   ├── file-manager.c
        │   │   ├── file-manager.h
        │   │   ├── question-file-manager.c
        │   │   ├── question-file-manager.h
        │   │   ├── topic-file-manager.c
        │   │   └── topic-file-manager.h
        │   ├── main.c
        │   ├── Makefile
        │   ├── protocol.h
        │   ├── tcp-manager
        │   │   ├── answer-submit-io.c
        │   │   ├── answer-submit-io.h
        │   │   ├── question-get-io.c
        │   │   ├── question-get-io.h
        │   │   ├── question-submit-io.c
        │   │   ├── question-submit-io.h
        │   │   ├── tcp-manager.c
        │   │   └── tcp-manager.h
        │   ├── topics
        │   │   └── TopicN1
        │   │       ├── questions
        │   │       │   ├── hjk
        │   │       │   │   ├── answers
        │   │       │   │   │   ├── answer_01
        │   │       │   │   │   │   ├── answer.txt
        │   │       │   │   │   │   ├── img.png
        │   │       │   │   │   │   ├── num.txt
        │   │       │   │   │   │   └── uid.txt
        │   │       │   │   ├── img.png
        │   │       │   │   ├── question.txt
        │   │       │   │   └── uid.txt
        │   │       │   └── QuestionN1
        │   │       │       ├── img.png
        │   │       │       ├── question.txt
        │   │       │       └── uid.txt
        │   │       └── uid.txt
        │   ├── udp-manager
        │   │   ├── list-questions-io.c
        │   │   ├── list-questions-io.h
        │   │   ├── list-topics-io.c
        │   │   ├── list-topics-io.h
        │   │   ├── propose-topic-io.c
        │   │   ├── propose-topic-io.h
        │   │   ├── register-io.c
        │   │   ├── register-io.h
        │   │   ├── udp-manager.c
        │   │   └── udp-manager.h
        │   └── usecases
        │       ├── usecases.c
        │       └── usecases.h
        │
        │
        ├── forum-user
        │   ├── constants.h
        │   ├── error-messages
        │   │   ├── input-error-messages.h
        │   │   └── server-error-messages.h
        │   ├── exceptions.h
        │   ├── file-manager
        │   │   ├── file-manager.c
        │   │   └── file-manager.h
        │   ├── main.c
        │   ├── Makefile
        │   ├── protocol-manager
        │   │   ├── client-manager.c
        │   │   ├── client-manager.h
        │   │   ├── client-tcp-manager.c
        │   │   ├── client-tcp-manager.h
        │   │   ├── client-udp-manager.c
        │   │   └── client-udp-manager.h
        │   ├── question
        │   │   ├── question.c
        │   │   ├── question.h
        │   │   └── question.o
        │   ├── topic
        │   │   ├── topic.c
        │   │   └── topic.h
        │   ├── user
        │   │   ├── user.c
        │   │   └── user.h
        │   └── vector
        │       ├── vector.c
        │       └── vector.h
        ├── Makefile
        └── readme.txt

5. Project Organization: 

    Protocols - Implements protocol messages.
    IO - Does all the parsing related to the input and the output.
    Usecases - Makes calls to functions on file manager depending if it has to do with 
            files or deals with it itself on the contrary.
    Entities - Has the data structures used throughout the project.
    File Manager - Has functions that are responsible on dealing with files and related
                system calls.

    Temporary - The tmp files are created (on the server) when the server is receiving 
                files and images from the user to make sure everything is received before
                the submission is accepted.



6. Directories Manipulation

    Whenever a topic is proposed, and it mets the constrains, a directory is created 
    with its name in the directory named "topics" which is inside the forum-server. 

        In this example, it's created a topic named "smtp" and along with it is created 
        an empty directory named "questions" and a uid.txt wich holds the information about 
        the user that created the topic.

            ├── forum-server
            │   ├── topics
            │   │   ├── smtp
            │   │   │   ├── questions
            │   │   │   └── uid.txt
   


    When a question is submited on a topic, a directory with its name is created inside the 
    questions directory and a directory "answers" is created as well.

        In this example, smtp topic is selected and question_submit is requested along with 
        question.txt (or named differently) and img.png (or named differently), when given 
        diferent name, server renames it on saving. uuid.txt is added in the question in 
        order to know the id of the question's owner.

            ├── forum-server
            │   ├── topics
            │   │   ├── smtp
            │   │   │   ├── questions
            │   │   │   │   ├── question01
            │   │   │   │   │   ├── answers
            │   │   │   │   │   ├── img.png
            │   │   │   │   │   ├── question.txt
            │   │   │   │   │   └── uid.txt
            │   │   │   │   ...
            │   │   │   └── uid.txt


    Finally, when an answer is submited on a question, a directory named "answer_01" ("01" if it 
    is the first answer) is created inside "answers".

        In this example, question01 is selected and answer_submit is requested along with 
        answer.txt and img.png. uuid.txt is added in the question in order to know the id 
        of the answer's owner. "num.txt" indicates the number of the question.

            ├── forum-server
            │   ├── topics
            │   │   ├── smtp
            │   │   │   ├── questions
            │   │   │   │   ├── question01
            │   │   │   │   │   ├── answers
            │   │   │   │   │   │   ├── answer_01
            │   │   │   │   │   │   │   ├── answer.txt
            │   │   │   │   │   │   │   ├── img.png
            │   │   │   │   │   │   │   ├── num.txt
            │   │   │   │   │   │   │   └── uid.txt
            │   │   │   │   │   │   ...
            │   │   │   │   │   ├── img.png
            │   │   │   │   │   ├── question.txt
            │   │   │   │   │   └── uid.txt
            │   │   │   │   ...
            │   │   │   └── uid.txt

