/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toni <toni@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 23:26:01 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/20 00:47:57 by toni             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iomanip>

namespace test_request
{

static const std::string text = "Hamlet: O what a rogue and peasant slave am I!\r\n \
Is it not monstrous that this player here, \r\n\
But in a fiction, in a dreame of passion, \r\n\
Could force his soule so to his own conceit \r\n\
That from her working all his visage wanned, \r\n\
Tears in his eyes, distraction in his aspect, \r\n\
A broken voice, and his whole function suiting \r\n\
With formes to his conceit; and all for nothing, \r\n\
For Hecuba! \r\n\
What's Hecuba to him, or he to her, \r\n\
That he should weepe for her? What would he doe \r\n\
Had he the Motive and the Cue for passion \r\n\
That I have? He would drowne the stage with teares, \r\n\
And cleave the generall eare with horrid speech, \r\n\
Make mad the guilty, and appale the free, \r\n\
Confound the ignorant, and amaze indeede \r\n\
The very faculties of eyes and eares; yet I, \r\n\
A dull and muddy-mettled rascal, peak \r\n\
Like John-a-dreames, unpregnant of my cause, \r\n\
And can say nothing; no not for a King, \r\n\
Upon whose property and most deare life \r\n\
A damn'd defeate was made. Am I a coward? \r\n\
Who calls me “villaine”? Breakes my pate acrosse? \r\n\
Pluckes off my beard, and blowes it in my face? \r\n\
Tweeks me by the nose? Gives me the lie in the throate \r\n\
As deepe as to the lunges? Who does me this? ";

#define N_CHUNCKS 5

std::string chuncked_post()
{
    std::ostringstream stream;

    /* Method */
    stream << "POST ";
    /* TEST */
    stream << "/hamlet_chunked";
    /* HTTP version */
    stream << " HTTP/1.1";
    /* END START LINE */
    stream << "\r\n";
    /* Host header */
    stream << "Host: localhost";
    stream << "\r\n";
    /* Chuncked Header */
    stream << "Transfer-Encoding: chunked";
    /* END HEADERS*/
    stream << "\r\n\r\n";

    /* BODY */
    //std::istringstream body;
    // split text into 4 chuncks
    size_t total_size = text.size();
    size_t chunck_size = total_size / N_CHUNCKS;
    size_t start = 0;
    // generate chuncks and lenght in HEX
    for (int i = 0; i < N_CHUNCKS; i++)
    {
        stream << std::uppercase << std::hex << chunck_size;
        stream << "\r\n";
        stream << text.substr(start, chunck_size);
        stream << "\r\n";
        start += chunck_size;
    }
    // Add end of text
    stream << std::uppercase << std::hex << total_size % 4;
    stream << "\r\n";
    stream << text.substr(start, total_size % N_CHUNCKS);
    stream << "\r\n";
    /* END BODY */
    stream << "0\r\n\r\n";
    
    std::cout << YELLOW "SIZE : " RESET << stream.str().size() << std::endl;

    return (stream.str());
}

std::string classic_post()
{
    std::ostringstream stream;

    /* Method */
    stream << "POST ";
    /* TEST */
    stream << "/hamlet_classic";
    /* HTTP version */
    stream << " HTTP/1.1";
    /* END START LINE */
    stream << "\r\n";
    /* Host header */
    stream << "Host: localhost";
    /* END Host Header*/
    stream << "\r\n";
    /* Lenght Header */
    stream << "Content-Length: ";
    /* Calculate size */
    stream << text.size();
    /* END HEADERS*/
    stream << "\r\n\r\n";
    /* TEXT */
    stream << text;
    
    return (stream.str());
}

}   // END NAMESPACE