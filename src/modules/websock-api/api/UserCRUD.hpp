/*
    Copyright (C) 2014-2016 Islog

    This file is part of Leosac.

    Leosac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leosac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "api/CRUDResourceHandler.hpp"
#include "core/auth/AuthFwd.hpp"

namespace Leosac
{
namespace Module
{
namespace WebSockAPI
{
/**
 * CRUD Handler for Users.
 */
class UserCRUD : public CRUDResourceHandler
{
  private:
    UserCRUD(RequestContext ctx);

  public:
    static CRUDResourceHandlerUPtr instanciate(RequestContext);

  private:
    virtual std::vector<ActionActionParam>
    required_permission(Verb verb, const json &req) const override;

    virtual json create_impl(const json &req) override;

    /**
     * Retrieve information about a given user, or about all users.
     *
     * Request:
     *     + `user_id`: The user_id of the user we want to fetch.
     *        Note that the special value `0` can be used to request all known users.
     *        This field is required.
     *
     * Response:
     *     + ...
     */
    virtual json read_impl(const json &req) override;

    /**
     * Update information about a given user.
     *
     * Request:
     *     + `user_id`: The user_id of the user we want to update. Required.
     *     + `attributes`: A dictionnary of a user's attributes (firstname,
     *       lastname, etc...).
     *
     *     Accepted attributes:
     *         + firstname
     *         + lastname
     *         + email
     *
      * Response:
     *     + ...
     */
    virtual json update_impl(const json &req) override;

    virtual json delete_impl(const json &req) override;

    /**
     * Make sure the group's attribute is valid (by calling
     * GroupValidator) and that the name is unique.
     */
    void validate_and_unique(Auth::GroupPtr grp);
};
}
}
}
