/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_codes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 01:01:09 by sshakya           #+#    #+#             */
/*   Updated: 2022/03/25 01:03:07 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETURN_CODES_HPP
#define RETURN_CODES_HPP

// Successful responces
#define SUCCESS 200
#define CREATED 201
#define ACCEPTED 202
#define NO_CONTENT 204
#define RESET_CONTENT 205
// Client Error responces
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOT_FOUND 404
#define NOT_ALLOWED 405
#define NOT_ACCEPTABLE 406
#define TIMEOUT 407
#define CONFLICT 409
#define GONE 410
#define LENGHT_REQUIRED 411
#define PRE_CONDITION_FAIL 412
#define TOO_LARGE 413
// Server Error Responces
#define INTERNAL_ERROR 500
#define NOT_IMPLEMENTED 501
#define BAD_GATEWAY 502
#define SERVICE_UNAVAILABLE 503
#define HTTPV_NOT 505

#endif  // RETURN_CODES_HPP