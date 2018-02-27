#pragma once

#include <boost/property_tree/json_parser.hpp>

namespace boost_helper
{
    /**
     * A simple wrapper around Boost.PropertyTree for quick & dirty JSON access.
     *
     * @author CismonX
     */
    class json
    {
        using ptree = boost::property_tree::ptree;

        /// The Boost.PropertyTree instance with stores this JSON structure.
        ptree* data_;

        /// Whether this is the node root of JSON object.
        bool is_root_ = false;

    public:

        /// Trying to modify the value of root node of JSON.
        class root_modification_exception : public std::exception {};

        /// Range-based for support.
        class iterator : public ptree::iterator
        {
        public:
            explicit iterator(const ptree::iterator& other) : ptree::iterator(other) {}

            std::pair<std::string, json> operator*() const
            {
                return make_pair(dereference().first, json(&dereference().second));
            }
        };

        /**
         * Iterator begin.
         */
        iterator begin() const
        {
            return iterator(data_->begin());
        }

        /**
         * Iterator end.
         */
        iterator end() const
        {
            return iterator(data_->end());
        }

        /**
         * Create a new JSON object with the given string.
         */
        explicit json(const std::string& json_str) : is_root_(true)
        {
            data_ = new boost::property_tree::ptree;
            std::istringstream stream(json_str);
            read_json(stream, *data_);
        }

        /**
         * Create a JSON object with a ptree node.
         */
        explicit json(boost::property_tree::ptree* other) : data_(other) {}

        /**
         * Assign the value of this node with another node.
         */
        json& operator=(const json& other)
        {
            if (is_root_)
                throw root_modification_exception();
            *data_ = *other.data_;
            return *this;
        }

        /**
         * Assign the value of this node with a string.
         */
        json& operator=(const std::string& value)
        {
            if (is_root_)
                throw root_modification_exception();
            data_->put_value(value);
            return *this;
        }

        /**
         * Get child node.
         */
        json operator[](const std::string& key) const
        {
            if (data_->find(key) == data_->not_found())
                data_->add_child(key, boost::property_tree::ptree());
            return json(&data_->get_child(key));
        }

        /**
         * Add a string element to this node without key.
         */
        void add(const std::string& value)
        {
            ptree element;
            element.put_value(value);
            data_->push_back(make_pair("", element));
        }

        /**
         * Add a child node to this node without key.
         */
        void add(const json& value)
        {
            data_->push_back(make_pair("", *value.data_));
        }

        /**
         * Get string value of this node.
         */
        std::string val() const
        {
            return data_->get_value<std::string>();
        }
        
        /**
         * Check whether this node has a child with the specified key.
         */
        bool has(const std::string& key) const
        {
            return data_->find(key) != data_->not_found();
        }

        /**
         * Print this JSON node to a string.
         */
        std::string to_string() const
        {
            std::ostringstream stream;
            write_json(stream, *data_, false);
            return stream.str();
        }

        /**
         * Destructor.
         */
        ~json()
        {
            if (is_root_)
                delete data_;
        }
    };
}