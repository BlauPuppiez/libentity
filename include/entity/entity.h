#pragma once

#include "entity/map.h"
#include "entity/tree.h"

namespace ent
{
	class entity
	{
		public:
			
			template <class T> std::string to(bool pretty = false) 	{ return to_tree().to<T>(pretty); }
			template <class T> void from(std::string &value)		{ tree t = T::from(value); from_tree(t); }

			tree to_tree();
			void from_tree(tree &tree);

			virtual void create_map() = 0;

			
		protected:

			template <class T> void map(std::string name, T &reference)
			{
				this->mapping[name] = std::make_shared<vmap<T>>(reference);
			}

			template <class T> void map(std::string name, std::vector<T> &reference)
			{
				this->mapping[name] = std::make_shared<vmap<T>>(reference);
			}

			template <class T> void map(std::string name, std::map<std::string, T> &reference)
			{
				this->mapping[name] = std::make_shared<vmap<T>>(reference);
			}


		private:

			std::map<std::string, std::shared_ptr<vmapbase>> mapping;
	};

	template <> void entity::map(std::string name, std::vector<byte> &reference);

}
