/*
 *  SimpleOauth - A simple OAuth authentication library for Qt
 *
 *  Copyright (C) 2010 Gregory Schlomoff <gregory.schlomoff@gmail.com>
 *                     http://gregschlom.com
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#ifndef OAUTH_TOKEN_H
#define OAUTH_TOKEN_H

#include <QSharedData>
#include <QMultiMap>
#include <QString>
#include "simpleoauth_export.h"

class QUrl;

namespace OAuth {

class TokenPrivate;

class SIMPLEOAUTH_EXPORT Token
{
public:
	enum TokenType {
		InvalidToken,
		RequestToken,
		AccessToken
	};

	enum AuthMethod {
		HttpHeader,
		Sasl
	};

	enum HttpMethod {
		HttpGet,
		HttpPost,
		HttpPut,
		HttpDelete,
		HttpHead
	};

	Token();
	Token(const Token& other);
	Token &operator=(const Token&);
	~Token();

	void setType(Token::TokenType type);
	void setConsumerKey(const QString& consumerKey);
	void setConsumerSecret(const QString& consumerSecretKey);
	void setCallbackUrl(const QUrl& callbackUrl);
	void setTokenString(const QString& token);
	void setTokenSecret(const QString& tokenSecret);
	void setVerifier(const QString& verifier);
    void setService(const QString& service);

	Token::TokenType type() const;
	QString tokenString() const;
	QString tokenSecret() const;
    QString consString() const;
    QString consSecret() const;
    QString tokenService() const;
    QUrl tokenCallback() const;

	QByteArray signRequest(const QUrl& requestUrl,
	                       Token::AuthMethod authMethod = HttpHeader,
	                       Token::HttpMethod method = HttpGet,
                               const QMultiMap<QString, QString>& parameters = (QMultiMap<QString, QString>())) const;
                               

private:
	QString generateSignature(const QUrl& requestUrl, const QMultiMap<QString, QString>& requestParameters, HttpMethod method) const;
	QString hmac_sha1(const QString &message, const QString &key) const;
    QString plaintext(const QString& message, const QString& key) const;
    friend class TokenPrivate;
	QSharedDataPointer<TokenPrivate> d;
};
}

#endif // OAUTH_TOKEN_H
