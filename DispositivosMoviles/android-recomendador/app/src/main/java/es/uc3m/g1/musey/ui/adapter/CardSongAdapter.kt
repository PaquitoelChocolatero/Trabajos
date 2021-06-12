package es.uc3m.g1.musey.ui.adapter

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.navigation.findNavController
import androidx.recyclerview.widget.RecyclerView
import es.uc3m.g1.musey.NavigationDirections
import es.uc3m.g1.musey.databinding.AdapterSongBinding
import es.uc3m.g1.musey.model.api.lastfm.Track

class CardSongAdapter(
    var values: List<Track>,
) : RecyclerView.Adapter<CardSongAdapter.ViewHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val binding = AdapterSongBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return ViewHolder(binding)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = values[position]
        holder.setItem(item)
    }

    override fun getItemCount(): Int = values.size

    inner class ViewHolder(
        private val view: AdapterSongBinding,
    ) : RecyclerView.ViewHolder(view.root) {

        fun setItem(item: Track) {
            view.root.setOnClickListener {
                val directions = NavigationDirections.recommend(item)
                it.findNavController().navigate(directions)
            }
            view.track.track = item
        }
    }
}