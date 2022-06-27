/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cgi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 13:54:14 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/11 14:54:22 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>

int main(int argc, char **argv, char **env)
{
    write(1, "Content-Type: text/html; charset=ISO-8859-1\r\n", 45);
    write(1, "\r\n\r\n", 4);
    int i = 0;
    write(1, "<html>", 6);
    write(1, "<body>", 6);
    while (argv[i] != NULL)
    {
        write(1, argv[i], strlen(argv[i]));
        write(1, "<br>", 4);
        sleep(1);
        i++;
    }
    write(1, "<br>", 4);
    i = 0;
    while (env[i] != NULL)
    {
        write(1, env[i], strlen(env[i]));
        write(1, "<br>", 4);
        sleep(1);
        i++;
    }
    write(1, "<br>", 4);
    write(1, "</body>", 7);
    write(1, "</html>", 7);

    return 0;
}